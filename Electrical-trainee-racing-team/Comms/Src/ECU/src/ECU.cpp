// ECU.cpp
#include "ECU.h" 

// Configuring some constants
#define SERIAL_BAUD_RATE 115200
#define CAN_BAUD_RATE 500000 // Ensure this matches your CAN bus speed

// APPS Configuration
#define APPS_AGREEMENT_THRESHOLD 0.10 // 10% agreement threshold (Rule T11.8.4)
#define APPS_PRESSED_THRESHOLD 0.25   // Threshold to consider APPS active for some rules
#define APPS_LOW_THRESHOLD 0.05       // Pedal position <5% for resetting some latches / startup

// BPS Configuration 
// BPS_GMIN is defined in ECU.h
// Define a threshold for "hard braking" for T11.8.6, adjust based on your sensor.
#define BPS_HARD_BRAKE_RAW_THRESHOLD (BPS_GMIN + 200) // Example: BPS value indicating significant braking

// Motor Controller CAN Commands
#define TORQUE_COMMAND_BYTE 0x90       // Command byte for sending torque
#define MAX_TORQUE_REQUEST_SCALER 0x90 // Max value for torque request (scaled by APPS)

// Delays from user datasheet
#define RFE_ACKNOWLEDGEMENT_DELAY_MS 500 // Delay for MC to recognize RFE (T11.8.9 implication)
#define MOTOR_OFF_SEQUENCE_DELAY_MS 1000 // Delay in off sequence

// --- FSAE Rule T11.8 Related Constants & Timers ---
// loop time in milliseconds from delay(30)
#define LOOP_CYCLE_MS 30 

// T11.8.3 APPS Hardware Failure Detection

// Min plausible ADC reading for APPS (ex., for 10-bit ADC)
#define ADC_MIN_VALID 5    
// Max plausible ADC reading for APPS (ex., for 10-bit ADC)
#define ADC_MAX_VALID 1018

// T11.8.4 / T11.8.7 APPS Disagreement
#define APPS_DISAGREEMENT_DURATION_MS 100
#define APPS_DISAGREEMENT_MAX_CYCLES (APPS_DISAGREEMENT_DURATION_MS / LOOP_CYCLE_MS + 1) 

// T11.8.6 Brake / APPS Plausibility
//1 second
#define BPS_APPS_IMPLAUSIBILITY_DURATION_MS 1000 

// T11.8.9 Throttle Position at Startup

// APPS >25% at startup is not allowed
#define APPS_STARTUP_MAX_THRESHOLD 0.25 

// --- State Flags for ECU Logic ---
volatile bool cplus_state_changed_flag = false; // ISR sets this

// Fault Latch Flags (static to retain state across loop calls)
static bool apps_hw_fault_latched = false;
static bool apps_disagreement_fault_latched = false;
static bool bps_apps_fault_latched = false;
static bool startup_apps_check_failed_latch = false; // For T11.8.9

// Timer state for BPS/APPS implausibility check
static unsigned long bps_apps_implausibility_start_time = 0;
static bool bps_apps_implausibility_timing = false;

static bool initial_startup_complete = false; // To manage T11.8.9 only once per "power on" cycle effectively

// Function Prototypes
void handle_precharge_interrupt();
void sendCAN(uint32_t id, const char *data, uint8_t len);
void sendTorque(uint32_t id, uint8_t torque);

// ISR: Interrupt
void handle_precharge_interrupt() {
    cplus_state_changed_flag = true;
}

// Precharge Logic
void handle_precharge() {
    if (digitalRead(CPLUS_PIN) == LOW) { // Assuming CPLUS_PIN LOW means precharge relay active (motors can be enabled)
        if (!precharged) {
            on(); // Call on()
            precharged = true; // Set precharged state
            initial_startup_complete = false; // Reset startup check for next "power on"
            startup_apps_check_failed_latch = false; // Reset this latch too
            Serial.println("Precharge sequence ON complete. System precharged.");
        }
    } else { // CPLUS_PIN HIGH means precharge relay not active
        if (precharged) {
            off(); // Call off(). This also sets precharged = false.
            Serial.println("Precharge sequence OFF complete. System NOT precharged.");
        }
    }
    cplus_state_changed_flag = false; // Reset the flag
}

void on() {
    Serial.println("Starting ON sequence for motor controllers...");
    digitalWrite(RFE_PIN, HIGH);
    delay(RFE_ACKNOWLEDGEMENT_DELAY_MS); // Crucial delay (500ms from user snippet)
    digitalWrite(FRG_PIN, HIGH);
    Serial.println("ON sequence finished. RFE and FRG HIGH.");
}

void off() {
    Serial.println("Starting OFF sequence for motor controllers...");
    // Send zero torque immediately if we are turning off
    sendTorque(MC_LEFT_RECEIVE, 0x00);
    sendTorque(MC_RIGHT_RECEIVE, 0x00);

    digitalWrite(FRG_PIN, LOW);
    delay(MOTOR_OFF_SEQUENCE_DELAY_MS); // Delay (1000ms from user snippet)
    digitalWrite(RFE_PIN, LOW);
    precharged = false;         // System is no longer precharged
    enable_sent_left = 0;       // Reset enable flags
    enable_sent_right = 0;
    // Reset fault latches that should clear on power-off/MC off condition
    apps_hw_fault_latched = false; // Hardware faults might persist, but this example clears for next precharge cycle
    apps_disagreement_fault_latched = false;
    bps_apps_fault_latched = false;
    startup_apps_check_failed_latch = false;
    initial_startup_complete = false; // System will need to go through startup checks again
    Serial.println("OFF sequence finished. FRG and RFE LOW. System NOT precharged.");
}

// CAN Receive Handler 
void mc_read(const CAN_message_t &message) {
    if (message.id == APPS_ID) { 
        APPS1.raw = ((message.buf[1] & 0x03) << 8) + message.buf[0];
        APPS2.raw = ((message.buf[2] & 0x0F) << 6) + (message.buf[1] >> 2);
        BPS1.raw = ((message.buf[3] & 0x3F) << 4) + (message.buf[2] >> 4);
        BPS2.raw = (message.buf[4] << 2) + (message.buf[3] >> 6);
      
        APPS1.min = APPS_GMIN; APPS1.max = APPS_GMAX;
        APPS2.min = APPS_GMIN; APPS2.max = APPS_GMAX;
        BPS1.min = BPS_GMIN; BPS1.max = BPS_GMAX;
        BPS2.min = BPS_GMIN; BPS2.max = BPS_GMAX;
    }
}

// CAN Setup

void setup_can() {
    d_can.begin();
    d_can.setBaudRate(CAN_BAUD_RATE);
    d_can.enableFIFO();
    d_can.enableFIFOInterrupt();
    d_can.onReceive(mc_read);
    Serial.println("CAN Bus Initialized.");
}

// Arduino Setup
void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("ECU Starting...");

    pinMode(FRG_PIN, OUTPUT);
    pinMode(RFE_PIN, OUTPUT);
    pinMode(CPLUS_PIN, INPUT_PULLDOWN); // Or INPUT_PULLUP based on the circuit

    digitalWrite(FRG_PIN, LOW);
    digitalWrite(RFE_PIN, LOW);

    setup_can();

    attachInterrupt(digitalPinToInterrupt(CPLUS_PIN), handle_precharge_interrupt, CHANGE);

    if (!(APPS_GMAX > APPS_GMIN)) {
        Serial.println("FATAL ERROR: APPS_GMAX must be greater than APPS_GMIN! Halting.");
        while (1) { delay(1000); Serial.println("APPS CALIBRATION ERROR - HALTED"); }
    }
    if (!(BPS_GMAX > BPS_GMIN)) {
        Serial.println("FATAL ERROR: BPS_GMAX must be greater than BPS_GMIN! Halting.");
        while (1) { delay(1000); Serial.println("BPS CALIBRATION ERROR - HALTED"); }
    }


    Serial.println("Initial check of precharge state...");
    // Manually trigger precharge logic once at startup based on current CPLUS_PIN state
    // This ensures `precharged` status and on()/off() sequences are correctly initialized.
    if (digitalRead(CPLUS_PIN) == LOW) {
        if (!precharged) { on(); precharged = true; }
    } else {
        if (precharged) { off(); /* precharged already false */ }
    }
    if(precharged) Serial.println("System started in PRECHARGED state.");
    else Serial.println("System started in NON-PRECHARGED state.");


    Serial.println("ECU Setup Complete.");
    delay(100); // Short delay before loop
}

// Main Loop
void loop() {
    if (cplus_state_changed_flag) {
        handle_precharge(); // Process precharge logic if CPLUS_PIN changed state
    }

    // Sensor Data Processing
    if (APPS_GMAX > APPS_GMIN) {
        APPS1.norm = constrain((double)(APPS1.raw - APPS_GMIN) / (APPS_GMAX - APPS_GMIN), 0.0, 1.0);
        APPS2.norm = constrain((double)(APPS2.raw - APPS_GMIN) / (APPS_GMAX - APPS_GMIN), 0.0, 1.0);
    } else { // Should be caught in setup, but runtime safeguard
        APPS1.norm = 0.0; APPS2.norm = 0.0;
    }

    double current_apps_value = (APPS1.norm + APPS2.norm) / 2.0;

    // Safety Checks (FSAE T11.8)
    bool allow_torque_command = true; // Assume true, set to false on any fault

    // T11.8.3: APPS Hardware Sensor Failure (out of plausible range)
    bool apps1_hw_fail = (APPS1.raw < ADC_MIN_VALID && APPS1.raw != 0) || APPS1.raw > ADC_MAX_VALID; // Allow 0 if it's a valid off state
    bool apps2_hw_fail = (APPS2.raw < ADC_MIN_VALID && APPS2.raw != 0) || APPS2.raw > ADC_MAX_VALID;
    if (apps1_hw_fail || apps2_hw_fail) {
        apps_hw_fault_latched = true;
        if(apps1_hw_fail) Serial.println("FAULT: APPS1 HW Failure!");
        if(apps2_hw_fail) Serial.println("FAULT: APPS2 HW Failure!");
    }
    if (apps_hw_fault_latched) {
        allow_torque_command = false;
    }

    // T11.8.4 / T11.8.7: APPS Disagreement
    static int apps_disagree_counter = 0;
    bool apps_currently_agree = fabs(APPS1.norm - APPS2.norm) < APPS_AGREEMENT_THRESHOLD;

    if (!apps_hw_fault_latched) { // Only process disagreement if no HW(Hardware) fault
        if (!apps_currently_agree) {
            apps_disagree_counter++;
            if (apps_disagree_counter >= APPS_DISAGREEMENT_MAX_CYCLES) {
                apps_disagreement_fault_latched = true;
                Serial.println("FAULT: APPS Disagreement LATCHED!");
            }
        } else {
            apps_disagree_counter = 0; // Reset counter if they agree
            // Check for unlatching condition for disagreement fault
            if (apps_disagreement_fault_latched && current_apps_value < APPS_LOW_THRESHOLD) {
                apps_disagreement_fault_latched = false;
                Serial.println("INFO: APPS Disagreement fault UNLATCHED.");
            }
        }
    }
    if (apps_disagreement_fault_latched) {
        allow_torque_command = false;
    }

    // T11.8.6: Brake Pedal / APPS Plausibility
    // Using BPS_HARD_BRAKE_RAW_THRESHOLD for "hard braking"
    bool is_hard_braking = (BPS1.raw > BPS_HARD_BRAKE_RAW_THRESHOLD) || (BPS2.raw > BPS_HARD_BRAKE_RAW_THRESHOLD);
    bool is_apps_active_for_plausibility = current_apps_value > APPS_PRESSED_THRESHOLD;

    if (!apps_hw_fault_latched && !apps_disagreement_fault_latched) { // Only process if no overriding APPS faults
        if (is_hard_braking && is_apps_active_for_plausibility) {
            if (!bps_apps_implausibility_timing) {
                bps_apps_implausibility_timing = true;
                bps_apps_implausibility_start_time = millis();
            } else {
                if (millis() - bps_apps_implausibility_start_time > BPS_APPS_IMPLAUSIBILITY_DURATION_MS) {
                    bps_apps_fault_latched = true;
                    Serial.println("FAULT: BPS/APPS Implausibility LATCHED!");
                }
            }
        } else {
            bps_apps_implausibility_timing = false; // Condition not met, reset timer state
            // Check for unlatching condition for BPS/APPS fault
            if (bps_apps_fault_latched && current_apps_value < APPS_LOW_THRESHOLD) {
                bps_apps_fault_latched = false;
                Serial.println("INFO: BPS/APPS Implausibility fault UNLATCHED.");
            }
        }
    }
    if (bps_apps_fault_latched) {
        allow_torque_command = false;
    }
    
    // T11.8.9: Throttle Position at Startup (when precharged and system ready first time)
    if (precharged && !initial_startup_complete && allow_torque_command) { // allow_torque_command checks other latches
        if (current_apps_value > APPS_STARTUP_MAX_THRESHOLD) {
            startup_apps_check_failed_latch = true; // Latch this state
            Serial.println("FAULT: APPS > 25% at startup. Torque inhibited until APPS < 5%.");
        } else {
            initial_startup_complete = true; // Startup check passed or not applicable this time
            startup_apps_check_failed_latch = false; // Clear if it was set and condition met
            Serial.println("INFO: Startup APPS check passed.");
        }
    }
    // If startup check latched, ensure torque remains zero until APPS is low
    if (startup_apps_check_failed_latch) {
        allow_torque_command = false;
        if (current_apps_value < APPS_LOW_THRESHOLD) {
            startup_apps_check_failed_latch = false; // Unlatch
            initial_startup_complete = true; // Consider startup sequence passed now
            Serial.println("INFO: Startup APPS check fault UNLATCHED (APPS < 5%).");
        }
    }


    // Final Torque Decision 
    uint8_t torque_val = 0x00;
    if (precharged && allow_torque_command) {
        // If all checks pass, calculate torque
        torque_val = (uint8_t)(MAX_TORQUE_REQUEST_SCALER * current_apps_value);

        // Send MC enable commands if not already sent (and if precharged)
        // Note: 'on()' function (called via handle_precharge) sets RFE/FRG.
        // This logic here is more about re-sending the specific CAN enable message
        // if it's required by the MCs after a period of zero torque.
        // The global 'enable_send' array from ECU.h is used.
        if (!enable_sent_left) {
            sendCAN(MC_LEFT_RECEIVE, enable_send, sizeof(enable_send));
            enable_sent_left = 1;
            Serial.println("Enable command sent to MC Left.");
        }
        if (!enable_sent_right) {
            sendCAN(MC_RIGHT_RECEIVE, enable_send, sizeof(enable_send));
            enable_sent_right = 1;
            Serial.println("Enable command sent to MC Right.");
        }
    } else {
        // Torque is not allowed (not precharged or a fault is active)
        torque_val = 0x00;
        if (enable_sent_left || enable_sent_right) { // If they were enabled
             Serial.println("Torque not allowed. Resetting MC enable flags. Sending 0 torque.");
        }
        enable_sent_left = 0;
        enable_sent_right = 0;
        // Optionally, if !precharged and RFE/FRG are high, could call off() here,
        // but handle_precharge should manage the RFE/FRG pins based on CPLUS_PIN.
        // Printing reasons for no torque:
        if (!precharged) Serial.println("Debug: Torque 0 because NOT precharged.");
        if (apps_hw_fault_latched) Serial.println("Debug: Torque 0 due to APPS HW Fault.");
        if (apps_disagreement_fault_latched) Serial.println("Debug: Torque 0 due to APPS Disagreement Fault.");
        if (bps_apps_fault_latched) Serial.println("Debug: Torque 0 due to BPS/APPS Implausibility Fault.");
        if (startup_apps_check_failed_latch) Serial.println("Debug: Torque 0 due to Startup APPS Check Failure.");

    }

    sendTorque(MC_LEFT_RECEIVE, torque_val);
    sendTorque(MC_RIGHT_RECEIVE, torque_val);

    delay(LOOP_CYCLE_MS); // Maintain loop cycle time
}

// CAN Local Helpers
void sendCAN(uint32_t id, const char *data, uint8_t len) {
    msg.id = id;
    msg.len = len > 8 ? 8 : len; // Ensure length does not exceed 8
    memcpy(msg.buf, data, msg.len);
    for (int i = msg.len; i < 8; i++) { // Zero out remaining bytes
        msg.buf[i] = 0;
    }
    d_can.write(msg);
}

void sendTorque(uint32_t id, uint8_t torque_value) {
    msg.id = id;
    msg.len = 8; // Torque messages are often fixed length
    msg.buf[0] = TORQUE_COMMAND_BYTE;
    msg.buf[1] = torque_value;
    for (int i = 2; i < 8; i++) { // Zero out the rest of the buffer
        msg.buf[i] = 0;
    }
    d_can.write(msg);
}

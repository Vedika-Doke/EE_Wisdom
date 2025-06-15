#include <FlexCAN_T4.h>

// Create a CAN1 object using the FlexCAN_T4 library (CAN1 = hardware CAN controller on Teensy 4.1)
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
CAN_message_t msg;  // CAN message object used to send data

// Sensor input pins for wheel RPM
// These should be connected to the signal output of each wheel sensor (e.g. Hall effect, optical encoder)

const uint8_t FL_RPM_PIN = 2;  // Front Left
const uint8_t FR_RPM_PIN = 3;  // Front Right
const uint8_t RL_RPM_PIN = 4;  // Rear Left
const uint8_t RR_RPM_PIN = 5;  // Rear Right

// Volatile pulse counters
// These are modified inside interrupt routines and read in the main loop

volatile uint16_t fl_pulses = 0;
volatile uint16_t fr_pulses = 0;
volatile uint16_t rl_pulses = 0;
volatile uint16_t rr_pulses = 0;

// Interrupt service routines for counting wheel sensor pulses
// Every rising edge = 1 pulse = portion of a wheel rotation

void countFL() { fl_pulses++; }
void countFR() { fr_pulses++; }
void countRL() { rl_pulses++; }
void countRR() { rr_pulses++; }

void setup() {
  Serial.begin(9600);
  delay(1000);  // Give serial monitor time to open

  // Initialize CAN1 at 500 kbps — standard speed 
  can1.begin();
  can1.setBaudRate(500000);
  Serial.println("CAN1 Initialized at 500 kbps");

  // Set the input pins for RPM sensors
  pinMode(FL_RPM_PIN, INPUT_PULLUP);
  pinMode(FR_RPM_PIN, INPUT_PULLUP);
  pinMode(RL_RPM_PIN, INPUT_PULLUP);
  pinMode(RR_RPM_PIN, INPUT_PULLUP);

  // Attach interrupt to count pulses on each wheel sensor
  attachInterrupt(digitalPinToInterrupt(FL_RPM_PIN), countFL, RISING);
  attachInterrupt(digitalPinToInterrupt(FR_RPM_PIN), countFR, RISING);
  attachInterrupt(digitalPinToInterrupt(RL_RPM_PIN), countRL, RISING);
  attachInterrupt(digitalPinToInterrupt(RR_RPM_PIN), countRR, RISING);
}

void loop() {

  // 1. Read analog sensor values for pedals
  uint16_t accel = analogRead(A0);  // Accelerator pedal position (0–1023)
  uint16_t brake = analogRead(A1);  // Brake pedal position (0–1023)
  
  // 2. Safely read and reset pulse counts
  // Disable interrupts briefly to prevent race conditions

  noInterrupts();
  uint16_t fl = fl_pulses;
  uint16_t fr = fr_pulses;
  uint16_t rl = rl_pulses;
  uint16_t rr = rr_pulses;
  fl_pulses = fr_pulses = rl_pulses = rr_pulses = 0;  // Reset for next cycle
  interrupts();

  // 3. Calculate RPM from pulses
  // Loop runs every 100 ms, so multiply by 600 to scale up to pulses/minute = RPM

  uint16_t fl_rpm = fl * 600;
  uint16_t fr_rpm = fr * 600;
  uint16_t rl_rpm = rl * 600;
  uint16_t rr_rpm = rr * 600;

  // 4. Pack and send CAN message with ID 120
  // This frame includes: accelerator, brake, front left RPM, front right RPM

  msg.id = 120;  // CAN ID for first frame
  msg.len = 8;   // Standard CAN frame length in bytes

  msg.buf[0] = highByte(accel);
  msg.buf[1] = lowByte(accel);
  msg.buf[2] = highByte(brake);
  msg.buf[3] = lowByte(brake);
  msg.buf[4] = highByte(fl_rpm);
  msg.buf[5] = lowByte(fl_rpm);
  msg.buf[6] = highByte(fr_rpm);
  msg.buf[7] = lowByte(fr_rpm);

  can1.write(msg);  // Send the frame over CAN bus

  // 5. Pack and send CAN message with ID 121
  // This frame includes: rear left RPM, rear right RPM
  // Remaining bytes can be reserved or used for other data

  msg.id = 121;  // CAN ID for second frame
  msg.len = 8;

  msg.buf[0] = highByte(rl_rpm);
  msg.buf[1] = lowByte(rl_rpm);
  msg.buf[2] = highByte(rr_rpm);
  msg.buf[3] = lowByte(rr_rpm);
  msg.buf[4] = 0;  // Unused
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  can1.write(msg);  // Send second frame

  // 6. Debug output to serial monitor
  // Helps verify data is being read and calculated correctly

  Serial.print("Accel: "); Serial.print(accel);
  Serial.print(" | Brake: "); Serial.print(brake);
  Serial.print(" | FL RPM: "); Serial.print(fl_rpm);
  Serial.print(" | FR RPM: "); Serial.print(fr_rpm);
  Serial.print(" | RL RPM: "); Serial.print(rl_rpm);
  Serial.print(" | RR RPM: "); Serial.println(rr_rpm);

  delay(100);  // Run loop every 100 ms 
}

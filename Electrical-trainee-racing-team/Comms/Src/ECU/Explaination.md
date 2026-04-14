# Explaining ECU.cpp code

## 1. Static Fault Latch Flags:
- (apps_hw_fault_latched, apps_disagreement_fault_latched, bps_apps_fault_latched, startup_apps_check_failed_latch)
- These boolean flags are declared static so that their values persist across multiple executions of the loop() function. They are used to "latch" or remember fault conditions.
- Once a fault occurs and its corresponding flag is set to true, it typically remains true (and torque remains inhibited) until a specific unlatching condition is met (e.g., pedal goes to zero, or a power cycle for some faults).

## 2. Function Prototypes:
This section lists prototypes for functions that are defined later in this ECU.cpp file. 
- void handle_precharge_interrupt();
- void sendCAN(uint32_t id, const char *data, uint8_t len);
- void sendTorque(uint32_t id, uint8_t torque);

## 3. Interrupt Service Routine (ISR) - handle_precharge_interrupt
- Purpose: To immediately react to changes in the precharge relay status.
- Action: It sets the cplus_state_changed_flag to true.

## 4. CAN Sending Helper Functions

1. void sendCAN(uint32_t id, const char *data, uint8_t len): A generic function to send a CAN message.
  - id: The CAN ID for the message.
  - data: A pointer to an array of characters (bytes) containing the data payload.
  - len: The length of the data payload (number of bytes to send, max 8).
  - It populates the msg structure with the id, len, and copies the data into msg.buf.
  - It ensures len does not exceed 8 (max CAN data length) and zero-fills any remaining bytes in msg.buf for consistency.
  - d_can.write(msg): Transmits the message onto the CAN bus. <br/>
2. void sendTorque(uint32_t id, uint8_t torque_value):
  - Roughly similar to void sendCAN() 

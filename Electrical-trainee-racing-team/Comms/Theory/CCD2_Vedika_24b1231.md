# Controls, Communications and Data Acquisitions 2

## Nucleo-F103RB development board based on STM32F103RB microcontroller
STM32F is a family of 32-bit micro-controllers based on the ARM Cortex-M core and developed by ST Microelectronics. <br/>
- It is medium-density performance line family incorporates the high-performance Arm® Cortex®-M3 32-bit RISC core operating at a 72 MHz frequency, high-speed embedded memories (Flash memory up to 128 Kbytes and SRAM up to 20 Kbytes), and an extensive range of enhanced I/Os and peripherals connected to two APB buses.
- All devices offer two 12-bit ADCs, three general purpose 16-bit timers plus one PWM timer, as well as standard and advanced communication interfaces: up to two I2Cs and SPIs, three USARTs, an USB and a CAN.

![Nucleo](https://drive.google.com/uc?export=view&id=15R-LASwkWFvlrwMuXon3BkZIjwIefi5r)

## Questions
### 1. How many sensors do we use in our car to measure the Accelerator pedal travel?(Assume that we use the minimum number given in the rulebook) <br/>
Minimum 2 sensors

![Pedalsensors](https://drive.google.com/uc?export=view&id=1b988Zu9kFYOgAWDLvz36xTlgmWz-rB1_)

### 5. Read about the start-up sequence of the car from the EV 4.11 and EV 4.12 rulebook section and write down what you think is the start-up sequence
  1. Low Voltage System Activation: The driver activates the LVMS.
  2. Activate the Tractive System: The driver initiates the TS from within the cockpit. (TSMS)
     -  It essentially "arms" the high voltage system pending further checks and actions.
     -  What happens:
        - The vehicle's control system will typically run a series of self-checks (ex., checking for faults in the insulation monitoring device (IMD), shutdown circuit, accumulator, motor controller).
        - If checks pass, the pre-charge relay will close, allowing a gentle charging of the motor controller's capacitors through a resistor to prevent a large inrush current.
        - After a short delay (once capacitors are pre-charged), the main Tractive System contactors (Accumulator Isolation Relays - AIRs) close, connecting the high voltage battery directly to the motor controller.
  3. Enter Ready-to-Drive Mode: After the TS is active, the driver must perform distinct additional actions to enter "Ready to Drive Mode."
     - The requirement for pressing the brake pedal ensures the car is stationary and under control when the powertrain is enabled.
  4. Emit Ready-to-Drive Sound: Upon entering R2D mode, the vehicle emits a distinct sound to indicate readiness
     - The sound must be 80–90 dBA and easily recognizable 
  5. Operational Readiness: The vehicle is now ready to respond to accelerator inputs.
  6. Safety Shutdown: Opening the SDC will immediately deactivate R2D mode, ensuring safety.



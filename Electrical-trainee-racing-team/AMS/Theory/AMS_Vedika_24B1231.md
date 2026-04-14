# AMS: Accumulator Management System
## Battery Management System(BMS):
- Takes in voltage, temperature, etc. as inputs.
- SOC: State of Charge(phone battery)
- SOH: State of Health (current performance compared to its original)
- SOE: State of Envelope(safe operating parameters, how much current you can charge or discharge at, at any given time)

## BMS connections:
- BMS is connected to all points across cells to measure voltages
At those points, it also measures the whole voltage difference across the battery pack.
- Can measure the current entering/exiting the battery.
- BMS can disconnect the battery from the circuit if it detects any undesirable or unsafe state.

## Accumulator Configuration:
Our accumulator has a 96s5p configuration, meaning there are 96 cell modules connected in series and each cell module is made up of 5 cells connected in parallel. (total 480 Li-ion cells)<br/>
Each cell has a nominal voltage of 3.6V (max voltage-4.2V) which makes it a battery of maximum voltage of 403.2V <br/>
5 cells are connected in parallel to make a cell module. 16 of these are connected in series to build a stack. The accumulator consists of 6 such stacks in series. <br/>

### Quick quiz:- why do you think 5 cells are joined in parallel?
- In a parallel connection, voltage remains the same, but current (and thus capacity) adds up.
- Reduces current stress on individual cells (increases lifespan)

### Quiz:- Analyze why do you think we changed to the new configuration find all pros and cons?
![New vs old cell](https://drive.google.com/uc?export=view&id=1Um_0iQYMm2r-QbXdVkmXGgBXINccfyaW)
Model INR-21700-P45B are the newly used cells.
#### Pros-
- Increased capacity of the cell
- Lower typical impeadance
- Slightly smaller diameter and height
#### Cons-
- Temperature during charging could reache 60 degrees as compared to the 40 degrees in the old cells used

## Shutdown Circuit(SDC) {LV safety circuit}:
![Shutdown Circuit](https://drive.google.com/uc?export=view&id=11AcDPyD_dTeoiaD0cBJDEY70hD9A23q0)
- LVMS (Low Voltage Master Switch): A physical switch placed on the side of the car that activates the LV system of the car and sends 12V through the remaining shutdown circuit as well.
- When the Braking System Plausibility Device (BSPD) detects “hard braking” — large power draw by the motors when the driver simultaneously presses accelerator and brake, it triggers the SDC.
- IMD (Insulation Monitoring Device): If the resistance between LV and HV paths falls below a certain threshold, the SDC is opened.
- AMS monitors the cell voltages, temperatures, and currents and opens the SDC if any of these parameters go out of bounds.
- Inertia switch detects any heavy jerk on the car.(to detect crashes)
- Three shutdown buttons are present that can be opened by the driver or authorized personnel outside the car.
- Brake Over-Travel Switch (BOTS): Detects brake pedal abnormalities. If there's a brake failure, the pedal presses the switch. This triggers the shutdown circuit (SDC) to open.
- HVD (High Voltage Disconnct): A mechanism to physically remove an element of the TS path to stop the supply. (Tractive System (TS) refers to all high-voltage components and wiring involved in powering the motor that drives the vehicle)
- TSMS (Tractive System Master Switch): A physical switch placed on the side of the car which is the last switch before the AIRs. (AIRs: (Accumulator Isolation Relays) are high-voltage relays used to connect or disconnect the accumulator (battery pack) from the rest of the TS)
- The precharge circuitry and the AIRs control the precharge process and allow the accumulator voltage to reach the motor controllers.
- We require an Activation Logic and in our car ,we call it the HV RESET, a physical button in the cockpit that the driver presses after all the checks to finally make the vehicle HV active.

## Accumulator charging:
The charger takes AC input and delivers a constant 3.3 kW of power with up to 440 V DC output to charge the accumulator. <br/>
### Charger Control
Control of the charger is managed by BMS via CAN(Controller Area Network) communication. <br/>
Charging can be halted in either of two ways:
- By sending a CAN message to stop charging.
- By opening the SC, isolating the accumulator and disabling the charger's charge enable input.
### Charger connectors:
- Input Power
- Control I/O
- Output Power: The Output connector includes an interlock, which is part of the charger's shutdown circuit and must be engaged before the charger can start supplying power. <br/>
The HV connectors of the accumulator containers also have an interlock which is also part of the SC, and must be linked to the charger interlock for energization.
The charger's shutdown circuit comprises BMS error detection, IMD error detection, and a push-type emergency kill switch, along with the charger and HV interlocks. Additionally, two TSMPs are connected to the HV output of the charger via 15k ohm resistors. The kill switch is rated for 14V and 21A, with a diameter of 30 mm. Current flow through the same AIRs used for discharging, which are controlled by the Accumulator Control Unit (ACU) board. The precharge circuit remains inactive during charging.

### Question : How can I charge my battery with a dc off board charger of voltage 250V? Any additional component or some different charging topology do we need to either add/adapt to get the task done?

## PCB in AMS:

1. Master Board: (Control center of BMS)
   - Establishes iso-spi communication using LTC6820 microcontroller.
   - Trigger AMS control if the voltage or temperature of cell modules exceeds certain limits.
2. Slave Board:
   - Monitors cell voltage and temperature of a stack. Collects this data and sends it to the master board.
   - LTC6813 microcontroller is used.
3. Battery Pack Monitor(BPM):
   - Used to monitor the accumulator current, voltage, and charge monitor for HV Battery Packs
   - Inbuilt iso-spi communication interface to share the measured data back to the AMS master which triggers AMS error.
4. Busbar PCB:
   - The voltage busbar allows the cell voltages from the stack to reach the slave.
   - The temperature busbar allows the voltages across the thermistors of each cell module to reach the slaves.
![connection between boards](https://drive.google.com/uc?export=view&id=1BRFx-zeQ6h744fTdujrBSd3Cw2kNlbm0)
### Rulebook task:
![Rulebook screenshot](https://drive.google.com/uc?export=view&id=1g8-YQgT31RhSrC5J7Vq8nu07GbGYT38Z)
### Assignment 1:
1. Communication initialisation using microcontroller
2. Read value from ADC
3. Multiply it by the voltage factor (Resistances act as voltage dividers)
4. Print final result
#### VBATP (VBAT Positive)
– Connect this to the midpoint of your divider (the junction of R14/R17 in your schematic).
– It’s the positive measurement terminal for the pack voltage.
#### VBATM (VBAT Minus)
– It serves as the reference (ground) for the VBATP measurement and also powers the chip’s internal analog front‐end when measuring battery voltage.

## Communication:
Parallel connections require a lot of cables as they send each bit of the data at the same time. <br/>
In serial communication, each bit is sent one after another using a single cable. But more clock pulse are required to send the data, thus making this type of communication slower.
- Two broad categories of serial communication:
  - Synchronous: Clock is used
  - Asynchronous: Clock is NOT used
- Types:
  - UART(Universal Asynchronous Receiver-Transmitter):
    1. Uses one cable along with the ground reference.
    2. Common configuration for both Tx(transmitter) and Rx(receiver) is set beforehand.
    These configurations include:
        - Data Length
        - Start and stop bits
        - Trasmission speed (duration of one bit = 1/(baud rate))
  - I2C(Inter-integrated circuit):
    1. Synchronous multi-master, multi-slave, single-ended serial computer bus
    2. Two connections along with ground reference are required.
    3. Data is sent by SDA, and SDL is the clock.
    4. Data length and frequency need to be specified.
    5. The address of the slave is first sent, and then the data is sent. The slave with the particular address receives the data sent and saves data in its buffer.
  - SPI(Serial peripheral Interface):
    1. Cables are termed as:
       - MOSI: Master Out Slave In
       - MISO: Master In Slave Out
       - SS: Select (Instead of address used in I2C)
       - SClk: Clock
    2. Duplex: Data can both be sent and received

![SPI modes](https://drive.google.com/uc?export=view&id=1tJlokQk43sJ5Bl2SU8esquHUkmJPtyYV)
#### Cons of using SPI-
- Any external noise can distort the signals being carried in the wires.
- It is inconvenient to route 4 wires together between two points in the car.
- The distance over which SPI is reliable is very low.
### Iso-SPI
Due to the cons mentioned above concerning SPI communication, we use LTC6820 IC to interface SPI and iso-SPI and switch toa 2-wired communication. Further , wepair them with transformers to provide isolation. <br/>

![SPI-IsoSPI interface](https://drive.google.com/uc?export=view&id=1N1rpl9fsD4Kc1aFMq0wnvSfuQETny4U_)

#### Why are the wires twisted? <br/>
- Differential signals consist of two wires carrying equal and opposite signals.
- Twisting the wires ensures that both conductors experience the same electromagnetic interference (EMI) from the environment.
- Since the receiver only responds to the difference between the two signals, any common noise gets canceled out — this is called common-mode noise rejection.

#### Why does these iso spi signals do not need amplifiers even for distances as long as 100m.
- It uses differential signaling
- Operates at low speed
- Has proper termination (The use of proper termination (120 ohm resistor matching the cable’s characteristic impedance) minimizes signal reflections and maintains signal strength.
- Uses transformer isolation for signal integrity and safety.

### Daisy Chain Network:
We use a daisy chain network to communicate between the master and the slaves. <br/>
But we will be shifting onto a circular chain network. The reason why it is better is because even if the wire connection breaks at a point, the communication is not hindered since a communication path still exists for all the slaves.

![DaisyChain](https://drive.google.com/uc?export=view&id=1ICaFwZCeUAw2pzLkNnrjoDh_E0ay-v8q)

### Assignment 2- Demonstrate (on TinkerCAD / in the lab) SPI communication between 2 Arduino by transmitting some data from the master to the slave repeatedly after some interval.

## Cell chemistry and modeling:
### Cell Modeling:
To model a cell some experiments have to be conducted on it such as low current charge discharge test , HPPC test , EIS testing etc. now the results of this test are analyzed to make equivalent cell RC modelwhose parameters could be used to get a precise estimate of SOC when the car is running.

### State of charge(SOC):
The state of charge (SOC) of the cells, is defined as the available capacity (in Ah) and expressed as a percentage of its rated capacity. <br/>
The three methods that can be used to extract the SOC of the cell are:
- The Voltage method:
  - Reading of battery voltage is converted into equivalent SOC value
  - Voltage also depends on two other factors:
    - Current: A correction term is added to take this in account.
    - Temperature: Similarly, a correction term is added to take this in account.
  - Battery voltage needs to be constant to apply this method, which is difficult to achieve.
- Coulomb counting method:(Current integration/ ampere hour counting)
  - Current flowing across battery is measure and integrated over time.
  - Errors occur while measuring the current.
  - Initial SOC must be known.
- Kalman Filter Method-
  - Measurements are observed over time, and an algorithm is employed.
  - Accurate initialization and large computing capacity is required.

### State of Health(SOH):
It represents a measure of the battery’s ability to store and deliver electrical energy, compared with a new battery. <br/>
Charge and discharge cycles cause gradual degradation in performance, decreasing the SOH of the battery. <br/>
SOH determines when the old battery must be replaced. <br/>
Evaluating SOH of a battery requires the following parameters:
- Increase in cell impedance
- Decrease in capacity
- Self-discharging rate
- Number of charge-discharge cycles
- Age of battery

## Data Analysis:
All the data received from our BMS gets logged in the SD card in the master board and the data logger in the encoded format. <br/>
We must be able to decode this data, process it, visualize it, and lastly analyze it. <br/>
The processed data gives us information about surge currents and error cells. This information can help us in debugging errors and tuning our car. The data received will also help us calculate the SoC. <br/>

## Cell Balancing Techniques:
No two cells coming off the same assembly line are exactly the same. There are always manufacturing inconsistencies. Keeping this in mind, charging rates might be different for different cells. <br/>
Techniques employed for cell balancing:
- Passive cell balancing:
  - The cells with higher SOC's are drained to match the SOC level of the cell with the least SOC.
  - Pros:
    1. Simple Architecture
    2. Cost effective
  - Cons:
    1. Capacity of the pack is limited by the SOC of the weakest cell
    2. Thermal management needed
    3. Energy is wasted
- Active cell balancing:
  - Distibution to SOC so all the cells have an average SOC
  - Pros:
    1. Enhance efficiency
    2. Better performance
### Quick Quiz: Why not active cell balancing?
  - Cons:
    1. Increased complexity
    2. Higher cost
    3. Slower Balancing rate

### Quick Quiz: Assume we are using passive balancing. Should we balance the cells while we are charging (battery connected to charger) or discharging (running the car), or both?
For passive balancing, you should balance the cells during charging, especially as the battery approaches full charge. This ensures safe operation, maximized capacity, and minimal energy waste.

### Quick Quiz: On which PCB do you think we use this techniques to balance the cell?
The BPM board could be used for balancing as we can montior the current, voltage, etc. and make sure the values of SOCs are known.

## AMS codes flowchart:
![AMS chart](https://drive.google.com/uc?export=view&id=1w-_juqzyIenYka6MMJDnn5tOU6tlu_Bu)

  

  

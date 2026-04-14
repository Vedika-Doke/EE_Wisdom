# Controls, Communication and Data Acquisition Module

## Serial Communication Protocols:
The data bits are transmitted one at a time in sequential manner over the data bus or communication channel in Serial Communication.

### Types of communication protocols are:
- Serial- Bits are transferred sequentially, one after another.
- Parallel- All data bits are tranferred simultaneously (parallel-ly)

## Transmission modes in serial communication:
- Simplex Method:
  - One way communication.
  - Either of the medium i.e sender or receiver can be active at a time.
- Half-Duplex Method:
  - Both sender and receiver can be active but not at the same time.
  - If the sender is transmitting then receiver can accept but cannot send and similarly vice versa.
- Duplex Method:
  - Two way communication.
  - Both receiver and transmitter can send data to each other at the same time.

## Clock Synchronization:

This is further classified as:
### 1. Synchronous Serial Interface:
  - Point-to-Point Master-Slave Connection:
    - Communication is usually between a master and one or more slaves.
    - The master initiates and controls the communication.
    - Each device doesn't need its own independent bus; instead, they share a common bus.
  - Shared Clock and Data Lines
    - Data and clock signals are sent over separate wires.
    - Because the clock is shared both sender and receiver are perfectly synchronized.
    - There is no need to match baud rates manually, unlike asynchronous communication.
  - How Transmission Works:
    1. Transmitter Side:
    - It sends the clock signal alongside the data so the receiver knows when to read each bit.
    2. Receiver Side:
      - Uses the incoming clock signal to synchronously read each data bit.
      - After receiving all bits, it converts the serial data back to parallel form (e.g., into a byte).
![Syn serial](https://drive.google.com/uc?export=view&id=1oaWCAeYbEUztxyWzgm_fsoqxBX7sM3eF)

### 2. Asynchronous Serial Interface:
- An Asynchronous Serial Interface is a method of communication where data is sent one bit at a time (serially) without using a shared clock between the sender and receiver. Instead, both ends operate on independent clocks and synchronize using  special control bits added to the data stream.
- Common in Long-Distance Communication:
  - Because it requires fewer wires (no clock line), asynchronous interfaces are simpler and cheaper to implement over longer distances.
  - It’s more tolerant of timing differences, which makes it stable and reliable for many practical applications.
- How Communication Works
  1. Transmitter Side:
  - Adds control bits to each data frame:
    - Start Bit: Marks the beginning of a data byte.
    - Stop Bit(s): Marks the end of the data byte.
    - Parity Bit (optional): Used for basic error checking.
  - Sends the entire frame serially, using its own clock.
  2. Receiver Side:
  - Uses its own clock to sample the incoming data.
  - Detects the start bit to align itself.
  - Extracts the data bits, checks the parity bit (if present), and then reads the stop bit(s).
  - Reassembles the bits into parallel form for the system to process. <br/>

![Asyn serial](https://drive.google.com/uc?export=view&id=1TxJcUkvPyJAQLS_Bgb2kwu826XMx51MD)

## Important terms:

### Baud rate:
Baud rate is rate at which the data is transferred between the transmitter and receiver in the form of bits per second (bps).<br/>
The more the baud rate will be faster the data will be transferred at a time. <br/>
Also for the data communication the baud rate has to be same for both transmitter and receiver.

### Framing:
Framing is referred to the number of data bits to be sent from transmitter to receiver.

### Synchronisation:
Synchronisation bits tell the start and end of the data bits.

### Error Control:
There are many factors which affect and add the noise in the serial communication. To get rid of this error the parity bits are used where parity will check for even and odd parity.

## Synchronous Serial Protocols:

### 1. Serial Peripheral Interface(SPI):
Consists of 4 basic signal lines:
- MISO: Master In, Slave Out
- MOSI: MAster Out, Slave In
- SCK: Clock pulse
- SS: Slave select/Chip select : Enables the master device to select which slave it wants to communicate with.
- - Vcc and Ground wires are also required.<br/>
It has duplex communication (Data is sent and received simultaneously) <br/>
SPI has only one master and multiple slaves. All communications can only be initiated by the master.<br/>

#### To start communication:
Required device's SS must be low to communicate with the master. If it is high, it ignores the master.
![SPI](https://drive.google.com/uc?export=view&id=1ANrwe80m8p27MLtjfmIxROY7Gh4VUbl5)

### 2. Inter Integrated Communication(I2C):
Consists of two lines: (Both the lines must be connected to a positive supply using a pull up resistor.)
- SDA(Serial Data Line): Sending and receiving data.
- SCL(Serial Clock Line): Clock Signal <br/>

It is used for short distance communication.<br/>
At any given time only the master will be able to initiate the communication. <br/>
Since there is more than one slave in the bus, the master has to refer to each slave using a different address. When addressed only the slave with that particular address will reply back with the information while the others keep quit. This way we can use the same bus to communicate with multiple devices.<br/>

The voltage levels of I2C are not predefined. I2C communication is flexible, means the device which is powered by 5v volt, can use 5v for I2C and the 3.3v devices can use 3.3v for I2C communication.<br/>
- But what if two devices which are running on different voltages, need to communicate using I2C?:
  - A 5V I2C bus can’t be connected with 3.3V device. In this case voltage shifters are used to match the voltage levels between two I2C buses. <br/>

1. There are some set of conditions which frame a transaction. Initialization of transmission begins with a falling edge of SDA, which is defined as ‘START’ condition in the diagram below where master leaves SCL high while setting SDA low.<br/>
2. The falling edge of SDA is the hardware trigger for the START condition. After this all devices on the same bus go into listening mode.<br/>
3. In the same manner, rising edge of SDA stops the transmission which is shown as ‘STOP’ condition in above diagram, where the master leaves SCL high and also releases SDA to go HIGH. So rising edge of SDA stops the transmission.
![I2C start n stop](https://drive.google.com/uc?export=view&id=1y2NUFxeCRdNmO2Ia6dCJpBFLLypjcVeL)

### 3. Universal Serial Bus(USB):
USB acts as "plug and play" device. <br/>
It uses differential signalling to reduce interference and allow high-speed transmission over a long distance.<br/>
A differential bus is built with two wire, one of represents the transmitted data and the other its complement. The idea is that the 'average' voltage on the wires does not carry any information, resulting in less interference.
#### Common-mode noise immunity:
- Any external interference (say, from nearby power lines or switching circuits) tends to affect both wires equally.
- Since the receiver only cares about the difference, common noise gets subtracted out.
#### No information in average voltage:
- Because the average voltage is constant and carries no useful data, it doesn't fluctuate with the signal.
- Result: less signal radiation, less coupling into nearby circuits = less interference.

### 4. Microwire:
Full duplex communication.<br/>
The 3 lines are:
- SI (Serial Input): SI is input line to the microcontroller.
- SO (SerialOutput): SO is the serial output line.
- SK (Serial Clock): SK is the serial clock line. <br/>
Data is shifted out on the falling edge of SK, and is valued on the rising edge. SI is shifted in on the rising edge of SK.<br/>

### 5. Controller Area Network (CAN):
- It applies to the Data Link layer and the physical layer of the ISO standard architecture.
- Every electronic device (or node) that communicates via the CAN protocol is connected with one another through a common serial bus, which allows for the transfer of messages. <br/>
- The CAN protocol does not follow the master-slave architecture, which means every nodes has access to read and write data on the CAN bus. When the node is ready to send data, it checks the availability of the bus and writes a CAN frame onto the network. A frame is a structure that carries a meaningful sequence of bit or bytes of data within the network. <br/>

##### CAN Bus Components and Output Signal:
CAN Bus is made up two wires, CAN-H (CAN High) and CAN-L (CAN Low) which connect to all the devices in the network. The signals on the two CAN lines have the same sequence of data, but their amplitudes are opposite. <br/>
By sending the data in equal and opposite ways like this allows for greater noise immunity and therefore less chance of the data being corrupted.<br/>
![CAN H-L](https://drive.google.com/uc?export=view&id=1PFBmERRKE1VJC5bt5RkD259EiJEDHLQu)

##### CAN Controller:
Receives the transfer data from the microcomputer integrated in the control unit/device (also known as CAN Node). It processes this data and relays it to the CAN transceiver.<br/>
Also, the CAN controller receives data from the CAN transceiver, processes it and relays it to the microcomputer integrated in the control unit/device (CAN Node).<br/>

##### CAN Transceiver:
It is a transmitter and receiver in one. It converts the data which the CAN controller supplies into electrical signals and sends this data over
the data bus lines. <br/>
Also, it receives data and converts this data for the CAN controller.

#### CAN Data Bus Terminal: 
It is a resistor (R) typically of 120 ohms. It prevents data sent from being reflected at the ends and returning as an echo.

- Supplying Data: The CAN Node provides data to the CAN controller for transfer. 
- Sending Data: The CAN transceiver receives data from the CAN controller, converts it into electrical signals and sends them back into the network.
- Receiving Data: All other CAN Nodes networked with the CAN data bus become receivers.
- Checking Data: The CAN Node checks whether they require the data they have received for their functions or not.
- Accepting Data: If the received data is important, it is accepted and processed. If not, the received data is ignored. <br/>

All nodes using the CAN protocol receive a frame and depending on the node’s ID, the CAN “decides” whether or not to accept it. If multiple nodes send the message at the same time, the node with the highest priority (so, the lowest arbitration ID) receives the bus access. Lower priority nodes must wait until  the bus is available. <br/>

Binary values in CAN protocol are termed as dominant and recessive bits:
- CAN define the logic “0” as dominant bit.
- CAN define the logic “1” as recessive bit.
- In this system dominant bit always overwrites the recessive bit.

#### Message Framing:
Messages in CAN are sent in a format called frames. A frame is defined structure, carrying meaningful sequence of bit or bytes of data within the network. <br/>
![Frame](https://drive.google.com/uc?export=view&id=1aivs3-h10bEjHefM9vl2bVoh2tY7a69O)

-  SOF (Start of Frame bit): Indicates start of message. A dominant bit in the field marks the start of frame.
-  IDENTIFIER: It serves dual purpose one, to determine which node has access to the bus and second to identify the type of message.
-  RTR (Remote Transmission Request): It identifies whether it’s a data frame or a remote frame .RTR is dominant when it is a data frame and recessive when it is a remote frame.
-  IDE (Identifier Extension. It is used to specify the frame format. (Standard or extended)
-  DLC (Data Length Code) It is 4 bit data length code that contains the number of bytes being transmitted.
-  DATA: Used to store up to 64 data bits of application data to be transmitted.
-  CRC (Cyclic Redundancy Check): For error detection.
-  ACK (Acknowledge (ACK) field): When the data is received correctly the recessive bit in ACK slot is overwritten as dominant bit by the receiver.
-  EOF: End of Frame
-  IFS (Inter Frame Space): Specifies minimum number of bits separating consecutive messages. It provides the intermission between two frames and consists of three recessive bits known as intermission bits. This time allows nodes for internal processing before the start of next frame.

#### Arbitration:
- If two nodes try to occupy the bus simultaneously, access is implemented with a nondestructive, bit-wise arbitration. Nondestructive means that the node winning arbitration just continues on with the message, without the message being destroyed or corrupted by another node. <br/>
- The lower the binary message identifier number, the higher its priority. An identifier consisting entirely of zeros is the highest priority message on a network because it holds the bus dominant the longest. <br/>
![Arbitration](https://drive.google.com/uc?export=view&id=1WiSkx-QT3MranHozsy0u8K4BU6NSKsmp)


## Asynchronous Serial Protocols:

Asynchronous communication does not require a timing clock that is common to both devices. Each device independently listens and sends digital pulses that represent bits of data at an agreed-upon rate. Asynchronous serial communication is sometimes referred to as Transistor-Transistor Logic (TTL) serial, where the high voltage level is logic 1, and the low voltage equates to logic 0. 

### Universal Asynchronous Receiver-Transmitter(UART) :
UART is a hardware communication protocol that uses asynchronous serial communication with configurable speed. <br/>
Asynchronous means there is no clock signal to synchronize the output bits from the transmitting device going to the receiving end.<br/>
Two signals of each UART frvice are:
- Transmitter (Tx)
- Receiver (Rx) <br/>

The transmitting UART is connected to a controlling data bus that sends data in a parallel form. <br/>
From this, the data will now be transmitted on the transmission line (wire) serially, bit by bit, to the receiving UART. This, in turn, will convert the serial data into parallel for the receiving device

![UART](https://drive.google.com/uc?export=view&id=1G-afrHRvcwe8gysjUbwYdJ3hRZR79u0x)

The point of synchronization is managed by having the same baud rate on both devices. <br/>
The Baud rate must be same for both the transmitting and receiving device. <br/>

![DAtaTransmission](https://drive.google.com/uc?export=view&id=1ecQ_Y6S_hKUry0kacmSK2Re6WnondBG3)

The UART data transmission line is normally held at a high voltage level when it’s not transmitting data. <br/>
To start the transfer of data, the transmitting UART pulls the transmission line from high to low for one (1) clock cycle. When the receiving UART detects the high to low voltage transition, it begins reading the bits in the data frame at the frequency of the baud rate. <br/>

## Questions:

### 1. Why do we need a CAN transceiver if the main data processing is done by the microcontroller?Clearly state the reasons and why it more advantageous than UART.(CAN transceiver used is SN65HVD230)
#### A CAN transceiver is needed because:
- The microcontroller (MCU) handles digital logic levels (usually 0–3.3V or 0–5v). CAN protocol, however, uses a differential voltage signaling (CAN_H and CAN_L) on a twisted pair — this is not directly compatible with the MCU. <br/>
- The transceiver ensures electrical compatibility, reliability, and protocol integrity.
- The transceiver converts logic-level signals from the MCU to differential signals suitable for the CAN bus, and vice versa.
- Noise Immunity and Long-Distance Communication is attained by the use of diffrential signaling.

#### Advantages of CAN over UART:
- Noise Immunity
- Multi-device support
- Error Handling
- Long distance communication possible.

### 2. What will happen if 2 nodes with the same ID number 120 start transmitting?
- Both nodes start sending the same ID bits (since they are identical).
- Neither loses arbitration, because the bits match.
- They continue transmitting, assuming they’ve won arbitration.
- As soon as the data field or CRC field differs (which it almost certainly will, unless the messages are identical in every bit), the CAN controller detects a bit error:
- One node transmits a bit it expects to see on the bus, but the other sends something different.
- This causes a CRC or bit error.
- CAN error handling kicks in.

### 4. Let's say you want to send 30 variables from a particular microcontroller (Node 1) to another microcontroller(Node 2) located in another PCB. Will you encode all the 30 variables using code? So what is your solution?(Hint:Find out more about DBC files, it makes life easier) 

A DBC file (short for Database CAN) is a file format used in CAN communication to describe how data is structured in CAN messages. It acts like a blueprint for interpreting raw CAN data into meaningful signals (like speed, temperature, RPM, etc.) <br/>


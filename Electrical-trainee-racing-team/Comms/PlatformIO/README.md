3. These are the variables you want to send to the main control unit of the car from
from the PCB responsible for all sensor values including the accelerator pedal
and brake sensors using CAN communication.
The variables 1 to 4 have to use CAN ID 120 and remaining used CAN ID 121.
Data to be sent in the dataframe of the CAN messages structure:
Variable 1: Accelerator Pedal Position(Varies from 0 to 1023)[Don't worry about the units]
Variable 2: Brake Pedal Position (Varies from 0 to 1023)
Variable 3: Front left wheel RPM (Varies from 0 to 6000)
Variable 4: Front right wheel RPM (Varies from 0 to 6000)
Variable 5: Rear right wheel RPM (Varies from 0 to 6000)
Variable 6: Rear right wheel RPM (Varies from 0 to 6000)
We use a library called FlexCAN to construct the structure of the CAN message on
PlatformIO and also to program the microcontroller to send and receive the
CAN messages just like UART communication uses Serial.begin() etc to start
communication.
Your task is to write the code for sending these variables through CAN lines on
PlatformIO with clear explanations. <br/>

Required code for question 3 is uploaded in <br/>
## Vedika Doke->CCD->VedikaPlatformIO->src->main.cpp

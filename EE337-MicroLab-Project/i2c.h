#ifndef I2C_H
#define I2C_H

// I2C Function 
void I2C_Init(void);        
void I2C_Start(void);                                      
void I2C_Stop(void);                                       
void I2C_Write(unsigned char byte);                 
unsigned char I2C_Read(void);                              
void I2C_Ack(void); // Send ACK after reading
void I2C_Nack(void);// Send NACK after reading
void I2C_RepeatedStart(void);                               

// Higher-level Device Driver Functions
unsigned char I2C_ReadRegister(unsigned char devAddr, unsigned char regAddr);    // Read 1 byte from device reg
void I2C_WriteRegister(unsigned char devAddr, unsigned char regAddr, unsigned char val); // Write 1 byte to device reg
void I2C_ReadMultiple(unsigned char devAddr, unsigned char regAddr, unsigned char *buf, unsigned char len); // Read multiple bytes

#endif

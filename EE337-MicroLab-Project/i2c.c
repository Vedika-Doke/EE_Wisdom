#include <at89c5131.h>
#include "i2c.h"

void I2C_Init(void){
	SSCON = 0xC5;
	return;
}

void I2C_Start(void){
	SSCON = SSCON | 0x20;    // STA=1
	SSCON = SSCON & 0xF7;    // SI=0
	while((SSCON & 0x08) == 0);  // Wait for SI=1
	SSCON = SSCON & 0xDF;
}

void I2C_Stop(void){
	SSCON = SSCON | 0x10;    // STO=1
	SSCON = SSCON & 0xF7;    // SI=0
	while(SSCON & 0x10);
}

void I2C_Write(unsigned char byte){
	SSDAT = byte;
	SSCON = SSCON & 0xF7;    // SI=0
	while((SSCON & 0x08) == 0);  // Wait until SI=1
}

unsigned char I2C_Read(void){
	SSCON = SSCON & 0xF7;    // SI=0
	while((SSCON & 0x08) == 0);  // Wait until SI=1
	return SSDAT;
}

void I2C_Ack(void){
	SSCON = SSCON | 0x04;    // AA=1
}

void I2C_Nack(void){
	SSCON = SSCON & 0xFB;    // AA=0
}

	//repeat start used when switching from write to read mode
void I2C_RepeatedStart(void){
	SSCON = SSCON | 0x20;    // STA=1
	SSCON = SSCON & 0xF7;    // SI=0
	while((SSCON & 0x08) == 0);  // Wait until SI=1
	SSCON = SSCON & 0xDF;
}


unsigned char I2C_ReadRegister(unsigned char devAddr, unsigned char regAddr){
	unsigned char val;
	I2C_Start();
	I2C_Write(devAddr << 1);  // Device address + Write_bit (0)
	I2C_Write(regAddr);    // Register address
	I2C_Start();
	I2C_Write((devAddr << 1) | 1);   // Device address + Read_bit (1)
	I2C_Nack();        // NACK after single byte read
	val = I2C_Read();
	I2C_Stop();
	return val;
}

void I2C_WriteRegister(unsigned char devAddr, unsigned char regAddr, unsigned char val){
	I2C_Start();
	I2C_Write(devAddr << 1);  // Device address + Write bit (0)
	I2C_Write(regAddr);     // Register address
	I2C_Write(val);    // Data to write
	I2C_Stop();
}

void I2C_ReadMultiple(unsigned char devAddr, unsigned char regAddr, unsigned char *buf, unsigned char len){
	unsigned char i;
	I2C_Start();
	I2C_Write(devAddr << 1);         // Device address + Write bit (0)
	I2C_Write(regAddr);              // Starting register address
	I2C_Start();
	I2C_Write((devAddr << 1) | 1);   // Device address + Read bit (1)
	for(i = 0; i < len; i++){
		if(i < len - 1){
			I2C_Ack();               // ACK for all bytes except last
		} else {
			I2C_Nack();              // NACK for last byte
		}
		buf[i] = I2C_Read();
	}
	I2C_Stop();
}
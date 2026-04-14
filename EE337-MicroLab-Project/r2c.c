#include <at89c5131.h>
#include "r2c.h"
#include "i2c.h"

#define SEC_REG  0x00
#define MIN_REG  0x01
#define HOUR_REG 0x02

void delay_ms(unsigned int time)
{
    int i, j;
    for(i=0;i<time;i++)
        for(j=0;j<382;j++);
}

void R2C_Init(void){
    delay_ms(200);
}

unsigned char BCD_to_Dec(unsigned char bcd){
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

unsigned char Dec_to_BCD(unsigned char dec){
    return ((dec / 10) << 4) | (dec % 10);
}

void R2C_SetTime(unsigned char sec, unsigned char min, unsigned char hour){
    I2C_WriteRegister(DS1307_ADDR, SEC_REG, Dec_to_BCD(sec) & 0x7F);
    I2C_WriteRegister(DS1307_ADDR, MIN_REG, Dec_to_BCD(min));
    I2C_WriteRegister(DS1307_ADDR, HOUR_REG, Dec_to_BCD(hour));
}

void R2C_GetTime(unsigned char *sec, unsigned char *min, unsigned char *hour){
    *sec  = BCD_to_Dec(I2C_ReadRegister(DS1307_ADDR, SEC_REG));
    *min  = BCD_to_Dec(I2C_ReadRegister(DS1307_ADDR, MIN_REG));
    *hour = BCD_to_Dec(I2C_ReadRegister(DS1307_ADDR, HOUR_REG));
}
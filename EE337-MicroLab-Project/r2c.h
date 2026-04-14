#ifndef R2C_H
#define R2C_H

#define DS1307_ADDR 0x68

void R2C_Init(void);
void R2C_SetTime(unsigned char sec, unsigned char min, unsigned char hour);
void R2C_GetTime(unsigned char *sec, unsigned char *min, unsigned char *hour);

unsigned char BCD_to_Dec(unsigned char bcd);
unsigned char Dec_to_BCD(unsigned char dec);

#endif
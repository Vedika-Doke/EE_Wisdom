#include <at89c5131.h>
#include "lcd.h"
#include "i2c.h"
#include "serial.h"
#include "r2c.h"

#define BMP280_ADDR       0x76
#define BMP280_ID_REG     0xD0
#define BMP280_CTRL_MEAS  0xF4
#define BMP280_TEMP_MSB   0xFA

unsigned char ID, val, calib[6], temp_raw[3];
unsigned int  dig_T1;
int           dig_T2, dig_T3;
long          adc_T, var1, var2, T;

unsigned char sec, min, hour;
unsigned char prev_sec = 255;

// ?? Helper function (since we cannot modify serial.h)
void uart_print_num(int num)
{
    unsigned char digits[10];
    int i = 0, j;

    if(num == 0){
        transmit_char('0');
        return;
    }

    if(num < 0){
        transmit_char('-');
        num = -num;
    }

    while(num > 0){
        digits[i++] = (num % 10) + '0';
        num /= 10;
    }

    for(j = i-1; j >= 0; j--){
        transmit_char(digits[j]);
    }
}

void main(void) {

    lcd_init();
    uart_init();
    I2C_Init();
    R2C_Init();
		R2C_SetTime(0,0,0);

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_write_string("BMP280 Test");
    msdelay(1000);

    ID = I2C_ReadRegister(BMP280_ADDR, BMP280_ID_REG);

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    if (ID == 0x58) {
        lcd_write_string("ID OK: 0x58");
    } else {
        lcd_write_string("ID Error");
    }

    I2C_WriteRegister(BMP280_ADDR, BMP280_CTRL_MEAS, 0x30);
    val = I2C_ReadRegister(BMP280_ADDR, BMP280_CTRL_MEAS);

    lcd_cmd(0xC0);
    if (val == 0x30) {
        lcd_write_string("Write OK");
    } else {
        lcd_write_string("Write FAIL");
    }

    msdelay(2000);

    I2C_ReadMultiple(BMP280_ADDR, 0x88, calib, 6);

    dig_T1 = ((unsigned int)calib[1] << 8) | calib[0];
    dig_T2 = ((int)calib[3] << 8) | calib[2];
    dig_T3 = ((int)calib[5] << 8) | calib[4];

    while (1) {
		
				R2C_GetTime(&sec, &min, &hour);

				if(sec != prev_sec) {   // update ONLY when second changes
						prev_sec = sec;

						// ---- Temperature ----
						I2C_WriteRegister(BMP280_ADDR, BMP280_CTRL_MEAS, 0x21);
						msdelay(50);

						I2C_ReadMultiple(BMP280_ADDR, BMP280_TEMP_MSB, temp_raw, 3);

						adc_T = ((long)temp_raw[0] << 12) |
										((long)temp_raw[1] << 4) |
										(temp_raw[2] >> 4);

						var1 = ((adc_T / 8) - ((long)dig_T1 * 2)) * ((long)dig_T2) / 2048;
						var2 = (adc_T / 16) - (long)dig_T1;
						var2 = (var2 * var2) / 4096 * ((long)dig_T3) / 16384;

						T = (var1 + var2) * 5 + 128;
						T = T / 256;

						// ---- LCD ----
						lcd_cmd(0x01);
						lcd_cmd(0x80);
						lcd_write_string("Time = ");
						lcd_print_num(hour);
						lcd_write_char(':');
						lcd_print_num(min);
						lcd_write_char(':');
						lcd_print_num(sec);

						lcd_cmd(0xC0);
						lcd_write_string("Temp = ");
						lcd_print_num(T / 100);
						lcd_write_char('.');
						if ((T % 100) < 10) lcd_write_char('0');
						lcd_print_num(T % 100);

						// ---- UART ----
						transmit_string("Time = ");
						uart_print_num(hour);
						transmit_char(':');
						uart_print_num(min);
						transmit_char(':');
						uart_print_num(sec);

						transmit_string(" Temp = ");
						uart_print_num(T / 100);
						transmit_char('.');
						uart_print_num(T % 100);
						transmit_string("\r\n");
				}
		}
	}
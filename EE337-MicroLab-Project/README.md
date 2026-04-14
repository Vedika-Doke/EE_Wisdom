# EE337 Microprocessors Lab - Course Project (Phase 3)

**IIT Bombay | EE337 Microprocessors Laboratory**

This project interfaces an AT89C5131 microcontroller with a BMP280 temperature sensor and DS1307 RTC over I2C, displaying time and temperature on a 16x2 LCD and logging via UART.

## Files

| File | Description |
|------|-------------|
| `main.c` | Main application logic |
| `i2c.c` / `i2c.h` | I2C (TWI) bus driver |
| `r2c.c` / `r2c.h` | DS1307 RTC driver |
| `lcd.h` | 16x2 HD44780 LCD driver |
| `serial.h` | UART serial communication (interrupt-driven) |
| `explanation.html` | Detailed code explanation with diagrams |

## View the Explanation

Open `explanation.html` in a browser or view it hosted at:  
**https://vedika-doke.github.io/EE_Wisdom/EE337-MicroLab-Project/explanation.html**

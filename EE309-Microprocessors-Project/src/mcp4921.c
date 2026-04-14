#include <at89c5131A.h>
#include "spi.h"
#include "mcp4921.h"

sbit DAC_CS = P1^1;

/* MCP4921 config nibble 0x7:
     bit 15 = 0  (write to DAC A)
     bit 14 = 1  (buffered VREF)
     bit 13 = 1  (gain 1x)
     bit 12 = 1  (active / !SHDN) */
#define DAC_CMD 0x7000

void dac_init(void) {
    DAC_CS = 1;
}

void dac_write(unsigned int val12) {
    unsigned int word = DAC_CMD | (val12 & 0x0FFF);
    DAC_CS = 0;
    spi_xfer((unsigned char)(word >> 8));
    spi_xfer((unsigned char)(word & 0xFF));
    DAC_CS = 1;
}

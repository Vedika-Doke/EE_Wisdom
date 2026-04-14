#include <at89c5131A.h>
#include "spi.h"
#include "mcp3008.h"

sbit ADC_CS = P1^0;   /* pick any free GPIO; rewire to match */

void adc_init(void) {
    ADC_CS = 1;
}

/* MCP3008 single-ended read, 3-byte frame:
     TX: 0x01, (1 << 7) | (ch << 4), 0x00
     RX: xx,   xxxxxx Dn, D8..D0
   10-bit result is bits[1:0] of byte2 ++ byte3. */
unsigned int adc_read(unsigned char channel) {
    unsigned char hi, lo;

    ADC_CS = 0;
    spi_xfer(0x01);
    hi = spi_xfer((unsigned char)(0x80 | ((channel & 0x07) << 4)));
    lo = spi_xfer(0x00);
    ADC_CS = 1;

    return ((unsigned int)(hi & 0x03) << 8) | lo;
}

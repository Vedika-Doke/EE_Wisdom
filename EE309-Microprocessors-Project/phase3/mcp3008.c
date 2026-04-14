#include <at89c5131.h>
#include "spi.h"
#include "mcp3008.h"

sbit ADC_CS = P1^0;

void adc_init(void) {
    ADC_CS = 1;
}

unsigned int adc_read(unsigned char channel) {
    unsigned char hi, lo;

    ADC_CS = 0;
    spi_xfer(0x01);
    hi = spi_xfer((unsigned char)(0x80 | ((channel & 0x07) << 4)));
    lo = spi_xfer(0x00);
    ADC_CS = 1;

    return ((unsigned int)(hi & 0x03) << 8) | lo;
}

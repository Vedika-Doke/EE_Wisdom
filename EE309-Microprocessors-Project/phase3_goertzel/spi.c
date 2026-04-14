#include <at89c5131.h>
#include "spi.h"

/* SPEN=1, SSDIS=1, MSTR=1, CPOL=0, CPHA=0, SPR2:1:0=001 -> Fosc/8 = 3 MHz */
#define SPCON_CFG 0x71

void spi_init(void) {
    unsigned char dummy;
    SPCON = SPCON_CFG;
    dummy = SPSTA;
    dummy = SPDAT;
}

unsigned char spi_xfer(unsigned char b) {
    SPDAT = b;
    while (!(SPSTA & 0x80));
    return SPDAT;
}

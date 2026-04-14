#include <at89c5131A.h>
#include "spi.h"

/* AT89C5131A SPI SFRs. If your device header already defines these,
   the #ifndef guards keep this compilable. Verify with the datasheet
   before flashing. */
#ifndef SPCON
sfr SPCON = 0xC3;
sfr SPSTA = 0xC4;
sfr SPDAT = 0xC5;
#endif

/* SPCON bit layout (AT89C5131A):
     7 SPR2 | 6 SPEN | 5 SSDIS | 4 MSTR | 3 CPOL | 2 CPHA | 1 SPR1 | 0 SPR0
   Config: SPEN=1, SSDIS=1 (P1.1 free for GPIO CS), MSTR=1,
           CPOL=0, CPHA=0 (MCP4921/MCP3008 mode 0,0),
           SPR2:1:0 = 001 -> Fosc/8 = 3 MHz at 24 MHz xtal
           (safe: MCP3008 max 3.6 MHz @5V; MCP4921 max 20 MHz). */
#define SPCON_CFG 0x71

void spi_init(void) {
    unsigned char dummy;
    SPCON = SPCON_CFG;
    dummy = SPSTA;   /* clear any stale SPIF from power-on */
    dummy = SPDAT;
}

unsigned char spi_xfer(unsigned char b) {
    SPDAT = b;
    while (!(SPSTA & 0x80));   /* wait SPIF */
    return SPDAT;
}

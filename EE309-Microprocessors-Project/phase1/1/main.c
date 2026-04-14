#include <at89c5131.h>
#include "spi.h"
#include "mcp4921.h"

/* NCO: Fs = 2000000/244 = 8196.7 Hz, f = 410 Hz
   delta = 65536 * 410 / 8196.7 = 3275
   Timer 0 mode 2 (8-bit auto-reload): TH0 = 256 - 244 = 12 */
#define TH0_RELOAD   12
#define PHASE_INC    3275u

unsigned char code sine32[32] = {
    128, 153, 177, 199, 218, 234, 245, 253,
    255, 253, 245, 234, 218, 199, 177, 153,
    128, 103,  79,  57,  38,  22,  11,   3,
      1,   3,  11,  22,  38,  57,  79, 103
};

volatile unsigned int phase = 0;

void timer0_isr(void) interrupt 1 {
    unsigned char idx;

    phase += PHASE_INC;
    idx = ((unsigned char)(phase >> 8)) >> 3;
    dac_write((unsigned int)sine32[idx] << 4);
}

void main(void) {
    CKCON0 &= ~0x01;        /* ensure standard timer clock (XTAL/12) */

    spi_init();
    dac_init();

    TMOD = (TMOD & 0xF0) | 0x02;  /* Timer 0 mode 2: 8-bit auto-reload */
    TH0  = TH0_RELOAD;
    TL0  = TH0_RELOAD;
    ET0  = 1;
    EA   = 1;
    TR0  = 1;

    while (1);
}

#include <at89c5131.h>
#include "spi.h"
#include "mcp4921.h"

/* NCO: Fs = 2000000/244 = 8196.7 Hz
   f1 = 410 Hz -> delta1 = 65536 * 410 / 8196.7 = 3275
   f2 = 470 Hz -> delta2 = 65536 * 470 / 8196.7 = 3754
   Timer 0 mode 2 (8-bit auto-reload): TH0 = 256 - 244 = 12 */
#define TH0_RELOAD    12
#define PHASE_INC_F1  3275u
#define PHASE_INC_F2  3754u

unsigned char code sine32[32] = {
    128, 153, 177, 199, 218, 234, 245, 253,
    255, 253, 245, 234, 218, 199, 177, 153,
    128, 103,  79,  57,  38,  22,  11,   3,
      1,   3,  11,  22,  38,  57,  79, 103
};

volatile unsigned int phase1 = 0;
volatile unsigned int phase2 = 0;

void timer0_isr(void) interrupt 1 {
    unsigned char s1, s2;
    unsigned int sum;

    phase1 += PHASE_INC_F1;
    phase2 += PHASE_INC_F2;

    s1 = sine32[((unsigned char)(phase1 >> 8)) >> 3];
    s2 = sine32[((unsigned char)(phase2 >> 8)) >> 3];

    /* sum two 0-255 values -> 0-510, shift left 3 to scale to 12-bit (0-4080) */
    sum = ((unsigned int)s1 + (unsigned int)s2) << 3;
    dac_write(sum);
}

void main(void) {
    CKCON0 &= ~0x01;

    spi_init();
    dac_init();

    TMOD = (TMOD & 0xF0) | 0x02;
    TH0  = TH0_RELOAD;
    TL0  = TH0_RELOAD;
    ET0  = 1;
    EA   = 1;
    TR0  = 1;

    while (1);
}

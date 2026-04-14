#include <at89c5131A.h>
#include "spi.h"
#include "mcp4921.h"
#include "mcp3008.h"

/* ======================================================================
   Mode select: 0 = single tone (410 Hz), 1 = two tones (410 + 470 Hz)
   ====================================================================== */
#define TWO_TONE_MODE  0

/* ----------------------------------------------------------------------
   NCO design:
     Xtal = 24 MHz, standard mode (12 clocks/cycle) -> 0.5 us/machine cycle
     F_s  = 16384 Hz -> 61.035 us/sample = 122 machine cycles
     Timer 0 mode 1 reload = 65536 - 122 = 65414 = 0xFF86

     Phase accumulator is 16-bit, wraps at 2^16 (= one full sine cycle).
     Phase increment per tick: delta = 65536 * f / F_s = 4 * f (exact).
       f1 = 410 Hz -> delta1 = 1640
       f2 = 470 Hz -> delta2 = 1880

     Sine LUT has 32 entries; top 5 bits of the phase index it.
     Index extraction: (unsigned char)(phase >> 8) >> 3
       This avoids an expensive 11-bit shift of a 16-bit value on the 8051.
   ---------------------------------------------------------------------- */
#define TH0_RELOAD    0xFF
#define TL0_RELOAD    0x86
#define PHASE_INC_F1  1640u
#define PHASE_INC_F2  1880u

/* CKCON controls X2 mode for timers. Declare here in case the device
   header uses a different name. Same-address duplicate is harmless in Keil. */
sfr CKCON_REG = 0x8F;

code unsigned char sine32[32] = {
    128, 153, 177, 199, 218, 234, 245, 253,
    255, 253, 245, 234, 218, 199, 177, 153,
    128, 103,  79,  57,  38,  22,  11,   3,
      1,   3,  11,  22,  38,  57,  79, 103
};

volatile unsigned int phase1 = 0;
volatile unsigned int phase2 = 0;

void timer0_isr(void) interrupt 1 {
    unsigned char s1;
#if TWO_TONE_MODE
    unsigned char s2;
    unsigned int  sum;
#endif

    TH0 = TH0_RELOAD;
    TL0 = TL0_RELOAD;

    phase1 += PHASE_INC_F1;
    s1 = sine32[((unsigned char)(phase1 >> 8)) >> 3];

#if TWO_TONE_MODE
    phase2 += PHASE_INC_F2;
    s2 = sine32[((unsigned char)(phase2 >> 8)) >> 3];
    sum = ((unsigned int)s1 + (unsigned int)s2) << 3;
    dac_write(sum);
#else
    dac_write((unsigned int)s1 << 4);
#endif
}

void main(void) {
    /* Force standard 8051 timer clock (XTAL/12).
       AT89C5131A supports X2 mode where timers run at XTAL/6.
       If the Pt-51 bootloader enables X2, all timing doubles.
       CKCON_REG (0x8F) bit 0 = T0X2. Clear it to ensure standard speed. */
    CKCON_REG &= ~0x01;

    spi_init();
    dac_init();
    adc_init();

    TMOD = (TMOD & 0xF0) | 0x01;  /* Timer 0 mode 1, preserve Timer 1 config */
    TH0  = TH0_RELOAD;
    TL0  = TL0_RELOAD;
    ET0  = 1;
    EA   = 1;
    TR0  = 1;

    while (1);
}

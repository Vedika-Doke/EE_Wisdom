#include <at89c5131.h>
#include "spi.h"
#include "mcp4921.h"
#include "mcp3008.h"
#include "serial.h"

#define TH0_RELOAD    12
#define PHASE_INC_F1  3275u
#define PHASE_INC_F2  3754u

unsigned char code sine32[32] = {
    128, 153, 177, 199, 218, 234, 245, 253,
    255, 253, 245, 234, 218, 199, 177, 153,
    128, 103,  79,  57,  38,  22,  11,   3,
      1,   3,  11,  22,  38,  57,  79, 103
};

volatile unsigned int ph1 = 0;
volatile unsigned int ph2 = 0;

#define N_TONES  4

/* Goertzel: Fs = 2000 Hz, N = 200 samples.
   All 4 target frequencies land on exact DFT bins:
     60 Hz  -> k = 6
     350 Hz -> k = 35
     410 Hz -> k = 41
     470 Hz -> k = 47
   Timer 2 reload = 65536 - (2000000/2000) = 65536 - 1000 = 64536 */
#define GOERTZEL_N       200
#define GOERTZEL_RELOAD  64536u

/* coeff = 2 * cos(2 * pi * k / N) */
float code goertzel_coeff[N_TONES] = {
    1.96352f,   /* k=6,  60 Hz  */
    0.90798f,   /* k=35, 350 Hz */
    0.55127f,   /* k=41, 410 Hz */
    0.19603f    /* k=47, 470 Hz */
};

void transmit_num(long n) {
    char buf[12];
    unsigned char i = 0;
    unsigned long u;

    if (n < 0) {
        transmit_char('-');
        u = (unsigned long)(-n);
    } else {
        u = (unsigned long)n;
    }

    if (u == 0) {
        transmit_char('0');
        return;
    }

    while (u > 0) {
        buf[i++] = '0' + (unsigned char)(u % 10);
        u /= 10;
    }
    while (i > 0) {
        transmit_char(buf[--i]);
    }
}

long measure_tone(unsigned char tone_idx) {
    float coeff, s0, s1, s2, power;
    unsigned int i;
    int x;

    coeff = goertzel_coeff[tone_idx];
    s1 = 0.0f;
    s2 = 0.0f;

    RCAP2H = (unsigned char)(GOERTZEL_RELOAD >> 8);
    RCAP2L = (unsigned char)(GOERTZEL_RELOAD & 0xFF);

    for (i = 0; i < GOERTZEL_N; i++) {
        TH2 = RCAP2H;
        TL2 = RCAP2L;
        TF2 = 0;
        TR2 = 1;
        ET0 = 0;
        x = (int)adc_read(0) - 512;
        ET0 = 1;
        while (!TF2);
        TR2 = 0;

        s0 = (float)x + coeff * s1 - s2;
        s2 = s1;
        s1 = s0;
    }

    power = s1 * s1 + s2 * s2 - coeff * s1 * s2;
    return (long)(power / 100.0f);
}

void timer0_isr(void) interrupt 1 {
    unsigned char s1, s2;
    unsigned int sum;

    ph1 += PHASE_INC_F1;
    ph2 += PHASE_INC_F2;
    s1 = sine32[((unsigned char)(ph1 >> 8)) >> 3];
    s2 = sine32[((unsigned char)(ph2 >> 8)) >> 3];
    sum = ((unsigned int)s1 + (unsigned int)s2) << 1;
    dac_write(sum);
}

void main(void) {
    unsigned char t;
    long results[N_TONES];

    CKCON0 &= ~0x01;

    spi_init();
    dac_init();
    adc_init();
    uart_init();

    TMOD = (TMOD & 0xF0) | 0x02;
    TH0 = TH0_RELOAD;
    TL0 = TH0_RELOAD;
    ET0 = 1;
    EA  = 1;
    TR0 = 1;

    transmit_string("\r\n--- Goertzel Tone Measurement ---\r\n");
    transmit_string("Fs=2000 Hz, N=200 samples\r\n\r\n");

    for (t = 0; t < N_TONES; t++) {
        results[t] = measure_tone(t);
    }

    transmit_string(" 60 Hz (f2-f1)  : ");
    transmit_num(results[0]);
    transmit_string("\r\n");

    transmit_string("350 Hz (2f1-f2) : ");
    transmit_num(results[1]);
    transmit_string("\r\n");

    transmit_string("410 Hz (f1)     : ");
    transmit_num(results[2]);
    transmit_string("\r\n");

    transmit_string("470 Hz (f2)     : ");
    transmit_num(results[3]);
    transmit_string("\r\n");

    transmit_string("--- Done ---\r\n");

    while (1);
}

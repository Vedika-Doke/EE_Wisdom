#include <at89c5131.h>

bit tx_complete = 0, rx_complete = 0;

void uart_init(void)
{
    TMOD = 0x20;
    TH1 = 0xF3;
    SCON = 0x50;
    TR1 = 1;
    ES = 1;
    EA = 1;
}

void transmit_char(unsigned char ch)
{
    SBUF = ch;
    while(!tx_complete);
    tx_complete = 0;
}

void transmit_string(unsigned char *s)
{
    while(*s != 0)
    {
        transmit_char(*s++);
    }
}

unsigned char receive_char(void)
{
    unsigned char ch = 0;
    while(!rx_complete);
    rx_complete = 0;
    ch = SBUF;
    return ch;
}

void serial_ISR(void) interrupt 4
{
    if(TI==1)
    {
        TI = 0;
        tx_complete = 1;
    }
    else if(RI==1)
    {
        RI = 0;
        rx_complete = 1;
    }
}

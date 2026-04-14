#ifndef MCP3008_H
#define MCP3008_H

void         adc_init(void);
unsigned int adc_read(unsigned char channel);   /* channel 0..7, returns 0..1023 */

#endif

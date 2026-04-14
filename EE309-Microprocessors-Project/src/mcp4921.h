#ifndef MCP4921_H
#define MCP4921_H

void dac_init(void);
void dac_write(unsigned int val12);   /* val12: 0..4095 */

#endif

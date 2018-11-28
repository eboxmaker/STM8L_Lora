#ifndef __MISC_H
#define __MISC_H
#include "ebox_core.h"

typedef union
{
    uint16_t val;
    uint8_t byte[2];

} xuint16_t;


extern char *ultoa( unsigned long value, char *string, int radix );
uint32_t ATOI32(char *str, int base);
char C2D(uint8_t c);
char D2C(uint8_t val);
uint32_t getPara(char **pPara, uint8_t base);
uint8_t digital2HexString(uint32_t val, uint8_t *buf);
void print_u32(uint32_t value);

#endif

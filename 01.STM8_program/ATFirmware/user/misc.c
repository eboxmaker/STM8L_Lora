#include "misc.h"
#include <stdlib.h>
extern char *ultoa( unsigned long value, char *string, int radix )
{
    char tmp[33];
    char *tp = tmp;
    long i;
    unsigned long v = value;
    char *sp;

    if ( string == NULL )
    {
        return 0;
    }

    if (radix > 36 || radix <= 1)
    {
        return 0;
    }

    while (v || tp == tmp)
    {
        i = v % radix;
        v = v / radix;
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'a' - 10;
    }

    sp = string;


    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;

    return string;
}

char C2D(
    uint8_t c	/**< is a character('0'-'F') to convert to HEX */
)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return 10 + c - 'a';
    if (c >= 'A' && c <= 'F')
        return 10 + c - 'A';

    return (char)c;
}
char D2C(
    uint8_t val
)
{
    if (val <= 9)
        return val + '0';
    if (val >= 10 && val <= 15)
        return  'A' + ( val - 10);
    return (char)val;
}

uint32_t ATOI32(
    char *str,	/**< is a pointer to convert */
    int base 	/**< is a base value (must be in the range 2 - 16) */
)
{
    uint32_t num = 0;
    while (*str != 0)
        num = num * base + C2D(*str++);
    return num;
}

uint32_t getPara(char **pPara, uint8_t base)
{
    char buf[15];
    int i = 0;
    if(**pPara == ',' || **pPara == '=' )(*pPara)++;//跳过前面非数字区域
    while(
        (**pPara >= '0' && **pPara <= '9') ||
        (**pPara >= 'a' && **pPara <= 'f') ||
        (**pPara >= 'A' && **pPara <= 'F')
    )
    {
        buf[i++] = **pPara;
        (*pPara)++;
    }
    buf[i++] = '\0';
    return  ATOI32((char *)buf, base);
}
/*
uint8_t digital2HexString(uint32_t val,uint8_t *buf)
{
    uint8_t i ;

    if(val <= 0xffff)
    {
        for(i = 0; i < 4; i++)
            buf[i] = D2C((val>>(12 - (i*4)))&0xf);
    }
    else
    {
        for(i = 0; i < 8; i++)
            buf[i] = D2C((val>>(28 - (i*4)))&0xf);
    }
    return (i);
}
*/
void print_u32(uint32_t value)
{
    char buf[10];
    buf[0] = value / 100 + 0x30;
    buf[1] = value / 10 % 10 + 0x30;
    buf[2] = value % 10 + 0x30;
    buf[3] = '\r';
    buf[4] = '\n';
    uart1_write(buf, 5);
}
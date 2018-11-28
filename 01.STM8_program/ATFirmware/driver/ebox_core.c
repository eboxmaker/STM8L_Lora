#include "ebox_core.h"

uint32_t milli_second = 0;

void ebox_reset()
{
    WWDG->CR |= 0x80;//WDGA=1
    WWDG->CR &= 0xbf;//T6ÖÃ0
}

void ebox_init()
{
    //CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    CLK->CKDIVR = (uint8_t)(CLK_SYSCLKDiv_1);

}

uint32_t millis()
{
    return milli_second;
}

void delay_ms(uint16_t ms)
{
    uint32_t now = milli_second;
    while(milli_second - now < ms)
    {
        nop();
    };
}

void delay_us(uint16_t us)
{
    while(us--)
    {
        nop();
    };
}
#ifndef __EBOX_CORE_H
#define __EBOX_CORE_H

#include "stm8l15x.h"


extern uint32_t milli_second;


void ebox_reset();
void ebox_init();
uint32_t millis();
void delay_ms(uint16_t ms);
void delay_us(uint16_t us);

#endif
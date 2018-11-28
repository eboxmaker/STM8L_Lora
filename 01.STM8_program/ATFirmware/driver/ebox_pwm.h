#ifndef __EBOX_PWM_H
#define __EBOX_PWM_H
#include "ebox_core.h"

void pwm1_config(uint8_t prescaler, uint16_t period, uint16_t pulse);
void pwm2_config(uint8_t prescaler, uint16_t period, uint16_t pulse);

#endif

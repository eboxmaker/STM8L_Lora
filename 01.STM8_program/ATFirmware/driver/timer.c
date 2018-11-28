#include "timer.h"
void tim4_config(void)
{
    /* TIM4 configuration:
     - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
     clock used is 16 MHz / 128 = 125 000 Hz
    - With 125 000 Hz we can generate time base:
        max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
        min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
    - In this example we need to generate a time base equal to 1 ms
     so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */
    //CLK_PeripheralClockConfig((CLK_Peripheral_TypeDef)CLK_Peripheral_TIM4, ENABLE);
    CLK->PCKENR1 |= CLK_PCKENR1_TIM4;//

    /* Time base configuration */
    //TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);//1ms period
    /* Set the Autoreload value */
    TIM4->ARR = (uint8_t)(124);
    /* Set the Prescaler value */
    TIM4->PSCR = (uint8_t)(TIM4_Prescaler_128);
    /* Generate an update event to reload the Prescaler value immediately */
    TIM4->EGR = TIM4_EventSource_Update;



    /* Clear TIM4 update flag */
    //TIM4_ClearFlag(TIM4_FLAG_Update);
    TIM4->SR1 = (uint8_t)(~((uint8_t)TIM4_FLAG_Update));



    /* Enable update interrupt */
    //TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    TIM4->IER |= (uint8_t)TIM4_IT_Update;

    /* enable interrupts */
    enableInterrupts();

    /* Enable TIM4 */
    //TIM4_Cmd(ENABLE);
    TIM4->CR1 |= TIM4_CR1_CEN ;

}
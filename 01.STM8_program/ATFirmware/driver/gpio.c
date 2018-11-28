#include "gpio.h"


#if 0

void gpio_pb0_init()
{
    GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
}
void gpio_pb0_write(uint8_t GPIO_BitVal)
{
    GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
    //CLK->PCKENR1 &= ~CLK_PCKENR1_TIM2;
    if (GPIO_BitVal != RESET)
    {
        GPIOB->ODR |= GPIO_Pin_0;
    }
    else
    {
        GPIOB->ODR &= (uint8_t)(~GPIO_Pin_0);
    }
}/*
void gpio_pb0_read()
{
    GPIO_Init(GPIOB,GPIO_Pin_0,GPIO_Mode_In_FL_IT);
}
*/
void gpio_pb0_toggle()
{
    GPIOB->ODR ^= GPIO_Pin_0;
    //GPIO_ToggleBits(GPIOB,GPIO_Pin_0);
}

void gpio_pc4_init()
{
    //GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Fast);
}

void gpio_pc4_write(uint8_t GPIO_BitVal)
{
    /*
    GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Fast);
    if (GPIO_BitVal != RESET)
    {
        GPIOC->ODR |= GPIO_Pin_4;
    }
    else
    {
        GPIOC->ODR &= (uint8_t)(~GPIO_Pin_4);
    }
    */
}
void gpio_pc4_toggle()
{
    /* //GPIO_ToggleBits(GPIOC,GPIO_Pin_4);
     GPIOC->ODR ^= GPIO_Pin_4;
     */

}
void gpio_pd0_init()
{
    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
}

void gpio_pd0_write(uint8_t GPIO_BitVal)
{
    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
    //CLK->PCKENR1 &= ~CLK_PCKENR1_TIM3;//关闭定时器PWM输出
    if (GPIO_BitVal != RESET)
    {
        GPIOD->ODR |= GPIO_Pin_0;

    }
    else
    {
        GPIOD->ODR &= (uint8_t)(~GPIO_Pin_0);
    }
}
void gpio_pd0_toggle()
{
    GPIOD->ODR ^= GPIO_Pin_0;
    //GPIO_ToggleBits(GPIOD,GPIO_Pin_0);
}
#endif
#include "low_power.h"
#include "usart.h"
#include "sx1278-hal.h"
void EXTI_InputPin_Init(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin)
{
    EXTI_Trigger_TypeDef EXTI_Trigger = EXTI_Trigger_Falling;

    GPIO_Init(GPIOx, GPIO_Pin, GPIO_Mode_In_PU_IT);


    EXTI->CR2 &=  (uint8_t)(~EXTI_CR2_P4IS);
    EXTI->CR2 |= (uint8_t)((uint8_t)(EXTI_Trigger) << ((uint8_t)EXTI_Pin_4 & (uint8_t)0xEF));

}

void GPIO_LowPower_Config(void)
{


    // Port ABCDEFG in output push-pull 0

    //GPIO_Init(GPIOA, GPIO_Pin_0|GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_In_PU_No_IT);

    //设置S1,S2为中断唤醒引脚，下降沿有效
    EXTI_InputPin_Init(GPIOC, GPIO_Pin_4);
    enableInterrupts();

}
void EnterHalt(void)
{


    /* Set GPIO*/

    GPIO_LowPower_Config();

    //CLK_LSICmd(DISABLE);
    //while ((CLK->ICKCR & 0x04) != 0x00);

    halt();
}

void ExitHalt(void)
{

    //使用外部时钟
    disableInterrupts();
    CLK->CKDIVR = (uint8_t)(CLK_SYSCLKDiv_1);
    //SX1278InitIo( );
    enableInterrupts();

    uart1_write_string("AT,WakeUp\r\n");

    //  RestorePinSetting();

}

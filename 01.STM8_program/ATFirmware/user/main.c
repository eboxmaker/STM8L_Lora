
#include "ebox.h"
#include "stdio.h"
#include "sx1278.h"
#include "at_cmd.h"
#include "at_port.h"
#include "low_power.h"

uint8_t halt_mode = 0;
const uint8_t info[] = "LoRa-01M(V0.6)\r\n";
uint8_t ack_on;
uint32_t ack_on_time;
extern uint8_t RFBuffer[];
extern uint8_t RxPacketSize;

extern at_stateType  at_state;
void ForwardPacket()
{
  disableInterrupts();
    // *size = RxPacketSize;
    //memcpy( ( void * )buffer, ( void * )RFBuffer, ( size_t )*size );
    uint8_t buf[8] ;
    uint8_t len = RxPacketSize;
    xuint16_t sourceAddr;
    xuint16_t destAddr;

    sourceAddr.byte[0] = *(RFBuffer + 0);
    sourceAddr.byte[1] = *(RFBuffer + 1);

    destAddr.byte[0] = *(RFBuffer + 2);
    destAddr.byte[1] = *(RFBuffer + 3);
    if(AddrEnable == 1)
    {
        len -= 4;
        if(LoRaAddr == destAddr.val || destAddr.val == 0xffff || LoRaAddr == 0xffff)
        {
            if(ack_on == 1)
            {
                //TIM3->CCER1 &= ~TIM_CCER1_CC2E;//输出使能
                //CLK->PCKENR1 &= ~CLK_PCKENR1_TIM3;//关闭定时器PWM输出
                GPIOD->DDR |= GPIO_Pin_0;//OUTPUT
                GPIOD->CR1 |= GPIO_Pin_0;//PP
                GPIOD->ODR |= GPIO_Pin_0;
                ack_on_time = millis();
            }

            uart1_write_string("LR,");
            buf[0] = D2C((sourceAddr.val & 0xF000) >> 12);
            buf[1] = D2C((sourceAddr.val & 0x0F00) >> 8);
            buf[2] = D2C((sourceAddr.val & 0x00F0) >> 4);
            buf[3] = D2C((sourceAddr.val & 0x000F) >> 0);
            buf[4] = ',';
            buf[5] = D2C((len & 0xF0) >> 4);
            buf[6] = D2C((len & 0x0F) >> 0);
            buf[7] = ',';

            uart1_write((uint8_t *)buf, 8);


            uart1_write((RFBuffer + 4), len );
            uart1_write_string("\r\n");
        }
    }
    else
    {
        if(ack_on == 1)
        {
            TIM3->CCER1 &= ~TIM_CCER1_CC2E;//输出使能
            CLK->PCKENR1 &= ~CLK_PCKENR1_TIM3;//关闭定时器PWM输出
            GPIOD->DDR |= GPIO_Pin_0;//OUTPUT
            GPIOD->CR1 |= GPIO_Pin_0;//PP
            GPIOD->ODR |= GPIO_Pin_0;
            ack_on_time = millis();
        }
        uart1_write((RFBuffer), len);
    }

    /*
    buf[0] = '-';
    buf[1] = RxPacketSize/100 + 0x30;
    buf[2] = RxPacketSize/10%10 + 0x30;
    buf[3] = RxPacketSize%10 + 0x30;
    buf[4] = '\r';
    buf[5] = '\n';
    uart1_write(buf,6);
    */

    RxPacketSize = 0;
    enableInterrupts();
}
uint8_t temp;

uint8_t is_RFBuffer_have_recv_data = 0;
void main(void)
{
    ebox_init();
    tim4_config();//初始化1ms定时器


    usart_init( USART1,
                115200,
                USART_WordLength_8b,
                USART_StopBits_1,
                USART_Parity_No,
                (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    uart1_write_string((uint8_t *)info);
    LoadConfig();
    SX1278Init();
    SX1278SetRFState(RFLR_STATE_RX_INIT);

    at_state = at_statTransportIdle;
    while (1)
    {
        switch( SX1278Process( ) )
        {
        case RF_IDLE:
            break;
        case RF_RX_TIMEOUT:
            uart1_write_string("AT,TimeOut\r\n");
            //printf("rx time:%ld\n",RxPacketTime);
            //SX1278SetRFState(RFLR_STATE_RX_INIT);
            //gpio_pb0_toggle();
            break;
        case RF_RX_WAIT_SIGNAL:
            if(at_state == at_statTransportRecvOver)
            {
                      disableInterrupts();

                TransportNewPacket();
                SX1278SendEnable();
                        enableInterrupts();

            }
            break;
        case RF_RX_RUNNING:
            //RxPacketSize = SX1278Read( SX1278_REG_RX_NB_BYTES );
            //uart1_write_string("x\r\n");

            break;

        case RF_RX_DONE:

            ForwardPacket();
            //uart1_write_string("+IPD");
            //uart1_write(buffer,bufferLength);

            //SX1278SetRFState(RFLR_STATE_RX_INIT);
            //SX1278SetTxPacket(buf,8);
            // gpio_pb0_toggle();
            break;
        case RF_TX_DONE:
            if(at_state == at_statTransportSending)
            {

                at_state = at_statTransportIdle;
                // uart1_write_string("SENDED\r\n");
            }
            //RFLRState = RFLR_STATE_RX_INIT;
            //SX1278SetRFState(RFLR_STATE_RX_INIT);
            //uart1_write_string("TX done\n");
            //SX1276LoRaSetRFState(RFLR_STATE_RX_INIT);
            else
            {
                uart1_write_string("AT,SENDED\r\n");
            }

            break;
        default:
            break;
        }

        at_process_loop();

        if(ack_on == 1)
        {
            if(millis() - ack_on_time >= 100)
                GPIOD->ODR &= (uint8_t)(~GPIO_Pin_0);
        }
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

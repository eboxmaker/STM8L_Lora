#include "at_fun.h"
#include "at.h"
#include "at_cmd.h"
#include "sx1278.h"
#include "misc.h"
#include "low_power.h"


extern at_stateType  at_state;

bool CheckPara(char *pPara)
{
    uint8_t dot = 0, err = 0;
    while(*pPara != '\r')
    {
        if(*pPara == ',' )
        {
            dot++;
        }
        else if(*pPara < '0' || *pPara > '9')
        {
            err = 1;
        }
        pPara++;

    }
    if(dot == 12 && err == 0)
        return TRUE;
    else
        return FALSE;
}

void at_CmdError()
{
    at_back(AT_ERR_CMD_ID);
    at_state = at_statIdle;
}
void at_CmdNull(char *pPara)
{
    at_back(AT_ERR_OK_ID);
    at_state = at_statIdle;
}

void at_CmdReset(char *pPara)
{
    at_back(AT_ERR_OK_ID);
    at_state = at_statIdle;
    WWDG->CR |= 0x80;//WDGA=1
    WWDG->CR &= 0xbf;//T6置0
}
void at_CmdVersion(char *pPara)
{
    at_back_para_ok(VERSION);
    at_state = at_statIdle;
}

/*
void at_CmdState(char *pPara)
{

 RFLR_STATE_RX_TIMEOUT,
 RFLR_STATE_TX_INIT,
 RFLR_STATE_TX_RUNNING,
 ,
 RFLR_STATE_TX_TIMEOUT,
 RFLR_STATE_CAD_INIT,
 RFLR_STATE_CAD_RUNNING,


 uint8_t state;
 if(*pPara == '?')
 {
     state = SX1278GetRFState();
     switch(state)
     {
     case RFLR_STATE_IDLE:
         uart1_write_string("IDLE\r\n");
         break;
     case RFLR_STATE_RX_INIT:
         uart1_write_string("RX_INIT\r\n");
         break;
     case RFLR_STATE_RX_RUNNING:
         uart1_write_string("RX_RUNNING\r\n");
         break;
     case RFLR_STATE_RX_DONE:
         uart1_write_string("RX_DONE\r\n");
         break;
     case RFLR_STATE_TX_DONE:
         uart1_write_string("TX_DONE\r\n");
         break;
     default:
         uart1_write_string("XXX");
         break;
     }
     at_back(AT_ERR_OK_ID);
 }
 else
 {
     at_back(AT_ERR_SYMBLE_ID);
 }
 at_state = at_statIdle;

}
*/

void at_CmdSleep(char *pPara)
{
    if(*pPara == '=')
    {
        pPara++;
        if(*pPara == '1')
        {
            SX1278SetRFState(RFLR_STATE_IDLE);
            //SX1278Reset();
            SX1278SetOpMode( SX1278_SLEEP );
            at_back(AT_ERR_OK_ID);
            EnterHalt();
            ExitHalt();
            //SX1278Init();

        }
        else
        {
            SX1278SetRFState(RFLR_STATE_RX_INIT);
            at_back(AT_ERR_OK_ID);

        }
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}
void at_CmdIdle(char *pPara)
{
    SX1278SetOpMode( SX1278_SLEEP );
    SX1278SetRFState(RFLR_STATE_IDLE);
    at_back(AT_ERR_OK_ID);
    at_state = at_statIdle;
}





void at_CmdConfig(char *pPara)
{
#if USE_READ_STATE
    uint8_t buf[8];
    uint8_t len;
#endif

    if(*pPara == '=')
    {
        pPara++;
        if(CheckPara(pPara) == FALSE)
        {
            at_back(AT_ERR_PARA_ID);
            at_state = at_statIdle;
            return ;
        }
        LoRaSettings.RFFrequency = getPara(&pPara, 10);
        LoRaSettings.Power = getPara(&pPara, 10);
        LoRaSettings.SignalBw = getPara(&pPara, 10);
        LoRaSettings.SpreadingFactor = getPara(&pPara, 10);
        LoRaSettings.ErrorCoding = getPara(&pPara, 10);
        LoRaSettings.CrcOn = (bool)getPara(&pPara, 10);
        LoRaSettings.ImplicitHeaderOn = (bool)getPara(&pPara, 10);
        LoRaSettings.RxSingleOn = (bool)getPara(&pPara, 10);
        LoRaSettings.FreqHopOn = (bool)getPara(&pPara, 10);
        LoRaSettings.HopPeriod = getPara(&pPara, 10);
        LoRaSettings.RxPacketTimeout = getPara(&pPara, 10);
        LoRaSettings.PayloadLength = getPara(&pPara, 10);
        LoRaSettings.PreambleLength = getPara(&pPara, 10);
        SX1278Init();
        at_state = at_statIdle;
        at_back(AT_ERR_OK_ID);

    }
    else if(*pPara == '?')
    {
#if USE_READ_STATE
        len = digital2HexString(LoRaSettings.RFFrequency, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.Power, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.SignalBw, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.SpreadingFactor, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.ErrorCoding, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.CrcOn, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.ImplicitHeaderOn, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.RxSingleOn, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.FreqHopOn, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.HopPeriod, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.RxPacketTimeout, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.PayloadLength, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string(",");
        len = digital2HexString(LoRaSettings.PreambleLength, buf);
        uart1_write((uint8_t *)buf, len);
        uart1_write_string("\r\n");
        at_back(AT_ERR_OK_ID);
#else
        at_back(AT_ERR_CMD_ID);
#endif


    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;

}
void at_CmdAddr(char *pPara)
{
    uint8_t buf[8];

    if(*pPara == '=')
    {
        LoRaAddr = 0;
        /*
        *pPara++;

        LoRaAddr |= (C2D(*pPara++) ) << 12;
        LoRaAddr |= (C2D(*pPara++) ) << 8;
        LoRaAddr |= (C2D(*pPara++) ) << 4;
        LoRaAddr |= (C2D(*pPara++) ) << 0;
        */
        LoRaAddr = getPara(&pPara, 16);
        at_back(AT_ERR_OK_ID);
    }
    else if(*pPara == '?')
    {

        buf[0] = D2C((LoRaAddr & 0xF000) >> 12);
        buf[1] = D2C((LoRaAddr & 0x0F00) >> 8);
        buf[2] = D2C((LoRaAddr & 0x00F0) >> 4);
        buf[3] = D2C((LoRaAddr & 0x000F) >> 0);
        buf[4] = 0;

        //len = digital2HexString(LoRaAddr,buf);
        at_back_para_ok(buf);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}
void at_CmdDestAddr(char *pPara)
{
    uint8_t buf[8];

    if(*pPara == '=')
    {
        DestAddr = getPara(&pPara, 16);
        at_back(AT_ERR_OK_ID);
    }
    else if(*pPara == '?')
    {

        buf[0] = D2C((DestAddr & 0xF000) >> 12);
        buf[1] = D2C((DestAddr & 0x0F00) >> 8);
        buf[2] = D2C((DestAddr & 0x00F0) >> 4);
        buf[3] = D2C((DestAddr & 0x000F) >> 0);
        buf[4] = 0;

        //len = digital2HexString(LoRaAddr,buf);
        at_back_para_ok(buf);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}

void at_CmdAddrEnable(char *pPara)
{

    uint8_t buf[8];

    if(*pPara == '=')
    {
        AddrEnable = getPara(&pPara, 10);
        if(AddrEnable != 0 && AddrEnable != 1)
            at_back(AT_ERR_PARA_ID);
        else
            at_back(AT_ERR_OK_ID);
    }
    else if(*pPara == '?')
    {

        buf[0] = D2C(AddrEnable);
        buf[1] = 0;

        //len = digital2HexString(LoRaAddr,buf);
        at_back_para_ok(buf);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}
void at_CmdSaveConfig(char *pPara)
{
    SaveConfig();
    at_back(AT_ERR_OK_ID);
    at_state = at_statIdle;
}
void at_CmdRxMode(char *pPara)
{
    SX1278SetRFState(RFLR_STATE_RX_INIT);
    at_back(AT_ERR_OK_ID);
    at_state = at_statIdle;
}
void at_CmdSend(char *pPara)
{
    if(*pPara == '=')
    {
        if(SX1278GetRFState() != RFLR_STATE_TX_RUNNING)
        {
            LoRaPacket.data_len = getPara(&pPara, 10);
            if(LoRaPacket.data_len > 0 && LoRaPacket.data_len < 250)
            {
                at_back(AT_ERR_OK_ID);
                at_state = at_statTranInit;
                return;
            }
            else
            {
                at_back(AT_ERR_PARA_ID);
            }
        }
        else
        {
            at_back(AT_ERR_RF_BUSY_ID);

        }
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;

}
void at_CmdGetRssi(char *pPara)
{

    uint8_t buf[6];
    int temp;
    if(*pPara == '?')
    {
        temp = -RxPacketRssiValue;
        buf[0] = '-';
        buf[1] = temp / 100 + 0x30;
        buf[2] = temp / 10 % 10 + 0x30;
        buf[3] = temp % 10 + 0x30;
        buf[4] = ',';
        buf[5] = '\0';
        at_back_para_ok(buf);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}
extern uint8_t ack_on;
void at_CmdAck(char *pPara)
{
    uint8_t buf[6];
    if(*pPara == '=')
    {
        pPara++;
        if(*pPara == '0')
        {
            ack_on = 0;
        }
        else
        {
            GPIOD->ODR &= (uint8_t)(~GPIO_Pin_0);
            ack_on = 1;
        }
        at_back(AT_ERR_OK_ID);
    }
    else if(*pPara == '?')
    {
        buf[0] = ack_on + 0X30;
        buf[1] = '\0';
        at_back_para_ok(buf);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;

}

void at_CmdTransport(char *pPara)
{
    at_state = at_statTransportIdle;
    if(SX1278GetRFState() == RFLR_STATE_IDLE)
      SX1278SetRFState(RFLR_STATE_RX_INIT);
    at_back(AT_ERR_OK_ID);
}
#if USE_IO

void at_CmdPB0(char *pPara)
{
    TIM2->CCER1 &= ~TIM_CCER1_CC1E;//输出使能
    CLK->PCKENR1 &= ~CLK_PCKENR1_TIM2;//关闭pwm输出

    if(*pPara == '=')
    {
        GPIOB->DDR |= GPIO_Pin_0;//OUTPUT
        GPIOB->CR1 |= GPIO_Pin_0;//PP
        //GPIOB->CR2 |= GPIO_Pin_0;//低速模式,默认值
        pPara++;
        if(*pPara == '0')
            GPIOB->ODR &= (uint8_t)(~GPIO_Pin_0);
        else
            GPIOB->ODR |= GPIO_Pin_0;
        at_back(AT_ERR_OK_ID);
    }
    else if(*pPara == '?')
    {
        GPIOB->DDR &= (uint8_t)(~(GPIO_Pin_0));//INPUT
        GPIOB->CR1 &= (uint8_t)(~(GPIO_Pin_0));//FLOAT
        GPIOB->CR2 &= (uint8_t)(~(GPIO_Pin_0));//无中断,默认值
        if(GPIOB->IDR & GPIO_Pin_0)
        {
            uart1_write_string("1,");
        }
        else
            uart1_write_string("0,");
        at_back(AT_ERR_OK_ID);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;

}
void at_CmdPC4(char *pPara)
{
    if(*pPara == '=')
    {
        GPIOC->DDR |= GPIO_Pin_4;//OUTPUT
        GPIOC->CR1 |= GPIO_Pin_4;//PP
        //GPIOC->CR2 |= GPIO_Pin_4;//低速模式,默认值

        pPara++;
        if(*pPara == '0')
            GPIOC->ODR &= (uint8_t)(~GPIO_Pin_4);
        else
            GPIOC->ODR |= GPIO_Pin_4;
        at_back(AT_ERR_OK_ID);
    }
    else if(*pPara == '?')
    {
        GPIOC->DDR &= (uint8_t)(~(GPIO_Pin_4));//INPUT
        GPIOC->CR1 &= (uint8_t)(~(GPIO_Pin_4));//FLOAT
        GPIOC->CR2 &= (uint8_t)(~(GPIO_Pin_4));//无中断,默认值

        if(GPIOC->IDR & GPIO_Pin_4)
        {
            uart1_write_string("1,");
        }
        else
            uart1_write_string("0,");
        at_back(AT_ERR_OK_ID);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}
void at_CmdPD0(char *pPara)
{
    ack_on = 0;//关闭Ack响应
    TIM3->CCER1 &= ~TIM_CCER1_CC2E;//输出使能
    CLK->PCKENR1 &= ~CLK_PCKENR1_TIM3;//关闭定时器PWM输出
    if(*pPara == '=')
    {
        GPIOD->DDR |= GPIO_Pin_0;//OUTPUT
        GPIOD->CR1 |= GPIO_Pin_0;//PP
        pPara++;
        if(*pPara == '0')
            GPIOD->ODR &= (uint8_t)(~GPIO_Pin_0);
        else
            GPIOD->ODR |= GPIO_Pin_0;
        at_back(AT_ERR_OK_ID);
    }
    else if(*pPara == '?')
    {
        GPIOD->DDR &= (uint8_t)(~(GPIO_Pin_0));//INPUT
        GPIOD->CR1 &= (uint8_t)(~(GPIO_Pin_0));//FLOAT
        GPIOD->CR2 &= (uint8_t)(~(GPIO_Pin_0));//无中断,默认值
        if(GPIOD->IDR & GPIO_Pin_0)
        {
            uart1_write_string("1,");
        }
        else
            uart1_write_string("0,");
        at_back(AT_ERR_OK_ID);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}

void at_CmdPWM1(char *pPara)
{
    uint8_t prescaler;
    uint16_t period;
    uint16_t pulse;
    if(*pPara++ == '=')
    {
        prescaler = getPara(&pPara, 10);
        period = getPara(&pPara, 10);
        pulse = getPara(&pPara, 10);
        pwm1_config(prescaler, period, pulse);
        at_back(AT_ERR_OK_ID);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}
void at_CmdPWM2(char *pPara)
{
    ack_on = 0;//关闭Ack响应
    uint8_t prescaler;
    uint16_t period;
    uint16_t pulse;
    if(*pPara++ == '=')
    {
        prescaler = getPara(&pPara, 10);
        period = getPara(&pPara, 10);
        pulse = getPara(&pPara, 10);
        pwm2_config(prescaler, period, pulse);
        at_back(AT_ERR_OK_ID);
    }
    else
    {
        at_back(AT_ERR_SYMBLE_ID);
    }
    at_state = at_statIdle;
}
#endif


#if USE_REG
void at_CmdReg(char *pPara)
{
    static uint8_t rawValue;
    uint8_t buf[6] = {0};
    static uint8_t reg;
    if(*pPara++ == ':')
        reg = getPara(&pPara);

    if(*pPara == '=')
    {
        rawValue = getPara(&pPara);
        SX1278Write(reg, rawValue);
        uart1_write_string(AT_ERR_OK_STR);
    }
    else if(*pPara++ == '?')
    {
        buf[0] = '(';
        buf[1] = '0';
        buf[2] = 'x';
        buf[3] = '0';
        buf[4] = '0';
        buf[5] = ')';
        rawValue = SX1278Read(reg);
        ultoa(rawValue, &buf[3], 16);
        buf[5] = ')';
        uart1_write(buf, 6);
        at_backOk;
    }
    else
    {
        at_backErrorCode(AT_ERR_SYMBLE);
    }
    at_state = at_statIdle;

}
#endif

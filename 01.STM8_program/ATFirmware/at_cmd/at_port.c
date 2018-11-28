#include "at_port.h"
#include "at_cmd.h"
#include "at.h"
#include "sx1278.h"
#define at_cmdLenMax 256

at_stateType  at_state = at_statIdle;

uint8_t at_cmdLine[at_cmdLenMax];



uint8_t Transport_counter = 0;
uint32_t Transport_last_time = 0;
uint32_t Transport_sending_note_flag = 0;
uint8_t Transport_exit_flag_pre = 0;

void at_recv_event(char temp)
{
    static uint8_t atHead[3];
    static uint8_t *pCmdLine;
    static uint16_t count = 0;
    switch(at_state)
    {
    case at_statIdle:
        atHead[0] = atHead[1];
        atHead[1] = temp;
        if(memcmp(atHead, "AT", 2) == 0)
        {
            at_state = at_statRecving;
            pCmdLine = at_cmdLine;
            atHead[1] = 0x00;
        }
        break;
    case at_statRecving: //push receive data to cmd line
        *pCmdLine = temp;
        if(temp == '\n')
        {
            pCmdLine++;
            *pCmdLine = '\0';
            at_state = at_statProcess;

        }
        else if(pCmdLine >= &at_cmdLine[at_cmdLenMax - 1])
        {
            at_state = at_statIdle;
        }
        pCmdLine++;
        break;
    case at_statProcess: //process data
        if(temp == '\n')
        {
            at_back(AT_ERR_CPU_BUSY_ID);
        }
        break;


    case at_statTranInit://no break;
        pCmdLine = at_cmdLine;
        at_state = at_statIpTraning;
    case at_statIpTraning:
        if(count  < (LoRaPacket.data_len) )
        {
            *pCmdLine++ = temp;
            count++;
        }

        if(count >= (LoRaPacket.data_len ))
        {
           if(AddrEnable == 1)
                LoRaPacket.packet_len = LoRaPacket.data_len + 4;
           else
                LoRaPacket.packet_len = LoRaPacket.data_len ;
           LoRaPacket.source.val = LoRaAddr;
            LoRaPacket.destination.val = DestAddr;
            LoRaPacket.data = (uint8_t *)at_cmdLine;
            SX1278SetTxPacket(&LoRaPacket);
            SX1278SetRFState(RFLR_STATE_TX_INIT);
            at_state = at_statIdle;
            count = 0;
            uart1_write_string("AT,SENDING\r\n");
        }
        break;
    case at_statTransportIdle://no break,进入透传后的初始化
        pCmdLine = at_cmdLine;
        Transport_counter = 0;
        Transport_sending_note_flag = 0;
        at_state = at_statTransportRecv;
    case at_statTransportRecv:
        if(Transport_counter <= 250)
        {
            pCmdLine[Transport_counter] = temp;
            Transport_counter++;
        }
        if( Transport_counter < 4)
        {

            atHead[0] = atHead[1];
            atHead[1] = atHead[2];
            atHead[2] = temp;
            Transport_last_time = millis();
            if(memcmp(atHead, "+++", 3) == 0)
            {
                Transport_exit_flag_pre = 1;
                atHead[2] = 0x00;
            }
        }
        Transport_last_time = milli_second;

        break;

    case at_statTransportRecvOver:
      
    case at_statTransportSending:
        if(Transport_sending_note_flag == 0)
        {
            Transport_sending_note_flag = 1;
            uart1_write_string("AT,busy...\r\n");
        }
        break;
    }



}

void at_process_loop()
{

    uint8_t temp;
    if(at_state == at_statProcess)
    {
      disableInterrupts();
        at_cmdProcess(at_cmdLine);
        enableInterrupts();
    }
    else if(at_state == at_statTransportRecv)
    {
        temp = milli_second - Transport_last_time;
        if(temp > 5)
        {
            if( Transport_exit_flag_pre == 1 )
            {
                Transport_exit_flag_pre = 0;

                at_state = at_statIdle;
                uart1_write_string("AT,OK\r\n");
                return ;
            }
            LoRaPacket.data_len = Transport_counter ;
           if(AddrEnable == 1)
                LoRaPacket.packet_len = Transport_counter + 4;
           else
                LoRaPacket.packet_len = Transport_counter ;


            LoRaPacket.source.val = LoRaAddr;
            LoRaPacket.destination.val = DestAddr;
            LoRaPacket.data = (uint8_t *)at_cmdLine;
            at_state = at_statTransportRecvOver;
        }
    }
    else if(at_state == at_statTransportSending)
    {

    }
}


void TransportNewPacket()
{

    SX1278SetTxPacket(&LoRaPacket);
    at_state = at_statTransportSending;
}
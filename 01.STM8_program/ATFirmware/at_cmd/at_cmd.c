#include "at_cmd.h"
#include "at_fun.h"
#include "string.h"

#define at_cmdNum   20
at_funcationType at_fun[at_cmdNum] =
{

    {NULL,          0,  at_CmdNull,     },
    {"+RST",        4,  at_CmdReset,    },
    {"+VER",        4,  at_CmdVersion,  },
    {"+SLEEP",      6,  at_CmdSleep,    },
    {"+IDLE",       5,  at_CmdIdle,     },
    {"+CFG",        4,  at_CmdConfig,   },
    {"+ADDR",       5,  at_CmdAddr,     },
    {"+DEST",       5,  at_CmdDestAddr, },
    {"+ADDREN",     7,  at_CmdAddrEnable,},
    {"+SAVE",       5,  at_CmdSaveConfig,   },
    {"+RX",         3,  at_CmdRxMode,   },
    {"+SEND",       5,  at_CmdSend,     },
    {"+RSSI",       5,  at_CmdGetRssi,  },
    {"+ACK",        4,  at_CmdAck,      },
    {"+TSP",        4,  at_CmdTransport,},

#if USE_IO
    {"+PB0",        4,  at_CmdPB0,      },
    {"+PC4",        4,  at_CmdPC4,      },
    {"+PD0",        4,  at_CmdPD0,      },
    {"+PWM1",       5,  at_CmdPWM1,     },
    {"+PWM2",       5,  at_CmdPWM2,     },
    {"+PWM2",       5,  at_CmdPWM2,     },
#endif

#if USE_REG
    {"+STATE",      6,  at_CmdState,    },
    {"+REG",        4,  at_CmdReg,      },
#endif



};
/**
  * @brief  Query and localization one commad.
  * @param  cmdLen: received length of command
  * @param  pCmd: point to received command
  * @retval the id of command
  *   @arg -1: failure
  */
static int16_t at_cmdSearch(int8_t cmdLen, uint8_t *pCmd)
{
    int16_t i;

    if(cmdLen == 0)
    {
        return 0;
    }
    else if(cmdLen > 0)
    {
        for(i = 1; i < at_cmdNum; i++)
        {
            //      os_printf("%d len %d\r\n", cmdLen, at_fun[i].at_cmdLen);
            if(cmdLen == at_fun[i].at_cmdLen)
            {
                //        os_printf("%s cmp %s\r\n", pCmd, at_fun[i].at_cmdName);
                if(memcmp(pCmd, at_fun[i].at_cmdName, cmdLen) == 0) //think add cmp len first
                {
                    return i;
                }
            }
        }
    }
    return -1;
}
static int8_t at_getCmdLen(uint8_t *pCmd)
{
    uint8_t n, i;

    n = 0;
    i = 128;

    while(i--)
    {
        if((*pCmd == '\r') || (*pCmd == '=') || (*pCmd == '?') || (*pCmd == ':'))
        {
            return n;
        }
        else
        {
            pCmd++;
            n++;
        }
    }
    return -1;
}
void at_cmdProcess(uint8_t *pAtRcvData)
{

    int16_t cmdId;
    int8_t cmdLen;

    cmdLen = at_getCmdLen(pAtRcvData);
    if(cmdLen != -1)
    {
        cmdId = at_cmdSearch(cmdLen, pAtRcvData);
    }
    else
    {
        cmdId = -1;
    }
    at_backHead;
    if(cmdId != -1)
    {
        pAtRcvData += at_fun[cmdId].at_cmdLen;
        if(at_fun[cmdId].at_Cmd)
        {
            at_fun[cmdId].at_Cmd((char *)pAtRcvData);
        }
        else
        {
            //        uart0_sendStr("no this fun\r\n"); //Relax, it's just a code.
            uart1_write_string("ERR\r\n");
        }
        /*
        //    os_printf("cmd id: %d\r\n", cmdId);
        pAtRcvData += at_fun[cmdId].at_cmdLen;
        if(*pAtRcvData == '\r')
        {
        if(at_fun[cmdId].at_setupCmd)
        {
          at_fun[cmdId].at_setupCmd((char *)pAtRcvData);
        }
        else
        {
        //        uart0_sendStr("no this fun\r\n"); //Relax, it's just a code.
          at_backError;
        }
        }
        else if(*pAtRcvData == '?' && (pAtRcvData[1] == '\r'))
        {
           if(at_fun[cmdId].at_setupCmd)
            {
              at_fun[cmdId].at_setupCmd((char *)pAtRcvData);
            }
            else
            {
        //        uart0_sendStr("no this fun\r\n"); //Relax, it's just a code.
              at_backError;
            }
        }
        else if((*pAtRcvData >= '0') && (*pAtRcvData <= '9') || (*pAtRcvData == '='))
        {
        if(at_fun[cmdId].at_setupCmd)
        {
          at_fun[cmdId].at_setupCmd((char *)pAtRcvData);
        }
        else
        {
        //        uart0_sendStr("no this fun\r\n"); //Relax, it's just a code.
          at_backError;
        }
        }
        else
        {
        at_backError;
        }*/
    }
    else
    {
        at_CmdError();
    }

}


void at_back(uint8_t err_id)
{
    switch(err_id)
    {
    case AT_ERR_OK_ID:
        uart1_write_string(AT_ERR_OK_STR);
        break;
    case AT_ERR_CMD_ID:
        uart1_write_string(AT_ERR_CMD_STR);
        break;
    case AT_ERR_CPU_BUSY_ID:
        uart1_write_string(AT_ERR_CPU_BUSY_STR);
        break;
    case AT_ERR_RF_BUSY_ID:
        uart1_write_string(AT_ERR_RF_BUSY_STR);
        break;
    case AT_ERR_SYMBLE_ID:
        uart1_write_string(AT_ERR_SYMBLE_STR);
        break;
    case AT_ERR_PARA_ID:
        uart1_write_string(AT_ERR_PARA_STR);
        break;
    }
}
void at_back_para_ok(uint8_t *str)
{
    uart1_write_string((uint8_t *)str);
    uart1_write_string(",OK\r\n");
}

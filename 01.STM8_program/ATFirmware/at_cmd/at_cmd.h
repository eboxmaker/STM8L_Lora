#ifndef __AT_CMD_H
#define __AT_CMD_H
#include "ebox.h"
#include "at.h"
#define VERSION "V0.6"



#define AT_ERR_OK_STR           "OK\r\n"
#define AT_ERR_CMD_STR          "ERR:CMD\r\n"
#define AT_ERR_CPU_BUSY_STR     "ERR:CPU_BUSY\r\n"
#define AT_ERR_RF_BUSY_STR      "ERR:RF_BUSY\r\n"
#define AT_ERR_SYMBLE_STR       "ERR:SYMBLE\r\n"
#define AT_ERR_PARA_STR         "ERR:PARA\r\n"

#define AT_ERR_OK_ID        0
#define AT_ERR_CMD_ID       1
#define AT_ERR_CPU_BUSY_ID  2
#define AT_ERR_RF_BUSY_ID   3
#define AT_ERR_SYMBLE_ID    4
#define AT_ERR_PARA_ID      5

#define at_backHead     uart1_write_string("AT,")
//#define at_backTeError   "+CTE ERROR: %d\r\n"

extern at_funcationType at_fun[];
void at_cmdProcess(uint8_t *pAtRcvData);
void at_back(uint8_t err_id);
void at_back_para_ok(uint8_t *str);

#endif
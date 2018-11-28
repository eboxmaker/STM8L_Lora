#ifndef __AT_FUN1_H
#define __AT_FUN1_H

#include "ebox.h"

#define USE_IO 0
#define USE_REG 0
#define USE_READ_STATE 0

void at_CmdError();
void at_CmdNull(char *pPara);
void at_CmdReset(char *pPara);
void at_CmdVersion(char *pPara);
void at_CmdState(char *pPara);
void at_CmdSleep(char *pPara);
void at_CmdIdle(char *pPara);

void at_CmdConfig(char *pPara);
void at_CmdAddr(char *pPara);
void at_CmdDestAddr(char *pPara);
void at_CmdAddrEnable(char *pPara);
void at_CmdSaveConfig(char *pPara);
void at_CmdRxMode(char *pPara);
void at_CmdSend(char *pPara);
void at_CmdGetRssi(char *pPara);

void at_CmdAck(char *pPara);

void at_CmdTransport(char *pPara);

#if USE_IO
void at_CmdPB0(char *pPara);
void at_CmdPC4(char *pPara);
void at_CmdPD0(char *pPara);
void at_CmdPWM1(char *pPara);
void at_CmdPWM2(char *pPara);
#endif
#if USE_REG
void at_CmdReg(char *pPara);
#endif
#endif

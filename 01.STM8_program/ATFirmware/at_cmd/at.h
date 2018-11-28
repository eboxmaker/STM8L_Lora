#ifndef __AT_H
#define __AT_H

typedef enum
{
    at_statIdle,
    at_statRecving,
    at_statProcess,
    at_statIpSending,
    at_statIpSended,
    at_statTranInit,
    at_statIpTraning,

    //透传模式状态机
    at_statTransportIdle,
    at_statTransportRecv,
    at_statTransportRecvOver,
    at_statTransportSending

} at_stateType;
typedef enum
{
    AtModeCMD,
    AtModeTransport

} AtMode_t;
typedef struct
{
    char *at_cmdName;
    int8_t at_cmdLen;
    void (*at_Cmd)(char *pPara);
} at_funcationType;

#endif
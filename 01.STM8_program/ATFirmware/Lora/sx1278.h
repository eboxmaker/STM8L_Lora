#ifndef __SX1278_H
#define __SX1278_H
#include "ebox.h"
#include "sx1278-hal.h"
#include "misc.h"
extern uint32_t RxTcPacketTime;
typedef struct sSX1276LR
{
    uint8_t RegFifo;                                // 0x00
    // Common settings
    uint8_t RegOpMode;                              // 0x01
    uint8_t RegRes02;                               // 0x02
    uint8_t RegRes03;                               // 0x03
    uint8_t RegBandSetting;                         // 0x04
    uint8_t RegRes05;                               // 0x05
    uint8_t RegFrfMsb;                              // 0x06
    uint8_t RegFrfMid;                              // 0x07
    uint8_t RegFrfLsb;                              // 0x08
    // Tx settings
    uint8_t RegPaConfig;                            // 0x09
    uint8_t RegPaRamp;                              // 0x0A
    uint8_t RegOcp;                                 // 0x0B
    // Rx settings
    uint8_t RegLna;                                 // 0x0C
    // LoRa registers
    uint8_t RegFifoAddrPtr;                         // 0x0D
    uint8_t RegFifoTxBaseAddr;                      // 0x0E
    uint8_t RegFifoRxBaseAddr;                      // 0x0F
    uint8_t RegFifoRxCurrentAddr;                   // 0x10
    uint8_t RegIrqFlagsMask;                        // 0x11
    uint8_t RegIrqFlags;                            // 0x12
    uint8_t RegNbRxBytes;                           // 0x13
    uint8_t RegRxHeaderCntValueMsb;                 // 0x14
    uint8_t RegRxHeaderCntValueLsb;                 // 0x15
    uint8_t RegRxPacketCntValueMsb;                 // 0x16
    uint8_t RegRxPacketCntValueLsb;                 // 0x17
    uint8_t RegModemStat;                           // 0x18
    uint8_t RegPktSnrValue;                         // 0x19
    uint8_t RegPktRssiValue;                        // 0x1A
    uint8_t RegRssiValue;                           // 0x1B
    uint8_t RegHopChannel;                          // 0x1C
    uint8_t RegModemConfig1;                        // 0x1D
    uint8_t RegModemConfig2;                        // 0x1E
    uint8_t RegSymbTimeoutLsb;                      // 0x1F
    uint8_t RegPreambleMsb;                         // 0x20
    uint8_t RegPreambleLsb;                         // 0x21
    uint8_t RegPayloadLength;                       // 0x22
    uint8_t RegMaxPayloadLength;                    // 0x23
    uint8_t RegHopPeriod;                           // 0x24
    uint8_t RegFifoRxByteAddr;                      // 0x25
    uint8_t RegModemConfig3;                        // 0x26
    uint8_t RegTestReserved27;                      // 0x27
    uint8_t RegFeiMsb;                              // 0x28
    uint8_t RegFeiMib;                              // 0x29
    uint8_t RegFeiLsb;                              // 0x2A
    uint8_t RegTestReserved2B[0x30 - 0x2B];         // 0x2B-0x30
    uint8_t RegDetectOptimize;                      // 0x31
    uint8_t RegTestReserved32;                      // 0x32
    uint8_t RegInvertIQ;                            // 0x33
    uint8_t RegTestReserved34[0x36 - 0x34];         // 0x34-0x36
    uint8_t RegDetectionThreshold;                  // 0x37
    uint8_t RegTestReserved38[0x3F - 0x38];         // 0x38-0x3F
    // I/O settings
    uint8_t RegDioMapping1;                         // 0x40
    uint8_t RegDioMapping2;                         // 0x41
    // Version
    uint8_t RegVersion;                             // 0x42
    // Test
    uint8_t RegTestReserved43;                      // 0x43
    // Additional settings
    uint8_t RegPllHop;                              // 0x44
    // Test
    uint8_t RegTestReserved45[0x4A - 0x45];         // 0x45-0x4A
    // Additional settings
    uint8_t RegTcxo;                                // 0x4B
    // Test
    uint8_t RegTestReserved4C;                      // 0x4C
    // Additional settings
    uint8_t RegPaDac;                               // 0x4D
    // Test
    uint8_t RegTestReserved4E[0x5A - 0x4E];         // 0x4E-0x5A
    // Additional settings
    uint8_t RegFormerTemp;                          // 0x5B
    // Test
    uint8_t RegTestReserved5C;                      // 0x5C
    // Additional settings
    uint8_t RegBitrateFrac;                         // 0x5D
    // Additional settings
    uint8_t RegTestReserved5E[0x60 - 0x5E];         // 0x5E-0x60
    // Additional settings
    uint8_t RegAgcRef;                              // 0x60
    uint8_t RegAgcThresh1;                          // 0x61
    uint8_t RegAgcThresh2;                          // 0x62
    uint8_t RegAgcThresh3;                          // 0x63
    // Test
    uint8_t RegTestReserved64[0x70 - 0x64];         // 0x64-0x70
} tSX1276LR;

typedef enum
{
    RF_IDLE,
    RF_BUSY,
    RF_RX_WAIT_SIGNAL,
    RF_RX_RUNNING,
    RF_RX_DONE,
    RF_RX_TIMEOUT,
    RF_TX_DONE,
    RF_TX_TIMEOUT,
    RF_LEN_ERROR,
    RF_CHANNEL_EMPTY,
    RF_CHANNEL_ACTIVITY_DETECTED,
} tRFProcessReturnCodes;

//LoRa
typedef enum
{
    RFLR_STATE_IDLE,
    RFLR_STATE_RX_INIT,
    RFLR_STATE_RX_WAIT_SIGNAL,
    RFLR_STATE_RX_RUNNING,
    RFLR_STATE_RX_DONE,
    RFLR_STATE_RX_TIMEOUT,
    RFLR_STATE_TX_INIT,
    RFLR_STATE_TX_RUNNING,
    RFLR_STATE_TX_DONE,
    RFLR_STATE_TX_TIMEOUT,
    RFLR_STATE_CAD_INIT,
    RFLR_STATE_CAD_RUNNING,
} tRFLRStates;
/*!
* SX1276 LoRa General parameters definition
*/
typedef struct sLoRaSettings
{
    uint32_t RFFrequency;
    int8_t Power;
    uint8_t SignalBw;                   // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
    // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
    uint8_t SpreadingFactor;            // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    uint8_t ErrorCoding;                // LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    bool CrcOn;                         // [0: OFF, 1: ON]
    bool ImplicitHeaderOn;              // [0: OFF, 1: ON]
    bool RxSingleOn;                    // [0: Continuous, 1 Single]
    bool FreqHopOn;                     // [0: OFF, 1: ON]
    uint8_t HopPeriod;                  // Hops every frequency hopping period symbols
    //uint32_t TxPacketTimeout;           //软件发送超时控制
    uint32_t RxPacketTimeout;           //软件接收超时控制
    uint8_t PayloadLength;              //用户数据长度：1-255
    uint16_t PreambleLength;            //前导码长度4-1024
} tLoRaSettings;



typedef struct
{
    xuint16_t source;
    xuint16_t destination;
    uint8_t *data;
    uint8_t data_len;
    uint8_t packet_len;
} Packet_t;

extern tLoRaSettings LoRaSettings;
extern int RxPacketRssiValue;
extern uint16_t LoRaAddr, DestAddr;
extern uint8_t AddrEnable ;
extern Packet_t LoRaPacket;

void SX1278Init();
void SX1278Reset();
void SX1278SetOpMode(uint8_t mode);
void SX1278ClearIRQFlags(uint8_t IrqFlagMask);
void SX1278SetRFFrequency(uint32_t freq);
uint32_t SX1278GetRFFrequency( void );
void SX1278SetRFPower( int8_t power );

void SX1278TxMode() ;
void SX1278Send(uint8_t *pBuffer, uint8_t len);
void SX1278SetTxPacket(Packet_t *packet);
void SX1278SendEnable();

void SX1278RxMode(bool RxSingleOn);
void SX1278GetRxPacket( void *buffer, uint8_t *size );

void SX1278SetRFState( uint8_t state );
uint8_t SX1278GetRFState();
uint8_t SX1278Process( void );
uint8_t SaveConfig();
uint8_t LoadConfig();

#endif
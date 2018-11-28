#ifndef __EBOX_EEPROM_H
#define __EBOX_EEPROM_H
#include "ebox_core.h"

#define EEPROM_STARTADDR 0x1000
uint8_t EEPROM_Read(uint16_t Addr, uint8_t *RxBuffer, int Length);
uint8_t EEPROM_Write(uint16_t Addr, uint8_t *RxBuffer, int Lenth);
#endif

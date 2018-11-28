#include "eeprom.h"

uint8_t EEPROM_Read(uint16_t Addr, uint8_t *RxBuffer, int Length)
{
    uint16_t T = 0;
    uint8_t *EEP;
    EEP = (uint8_t *)(EEPROM_STARTADDR + Addr);
    FLASH->DUKR = 0xAE;
    FLASH->DUKR = 0x56;
    FLASH->CR2 = 0x00;
    while((!(FLASH->IAPSR & 0x08)) & (T < 0xFFFF))T++;
    if(T == 0xFFFF)return 0;
    while(Length--)
    {
        *RxBuffer++ = *EEP++;
    }
    return 1;
}

uint8_t EEPROM_Write(uint16_t Addr, uint8_t *RxBuffer, int Lenth)
{
    uint16_t T = 0;
    uint8_t *EEP;
    EEP = (uint8_t *)(EEPROM_STARTADDR + Addr);
    FLASH->DUKR = 0xAE;
    FLASH->DUKR = 0x56;
    FLASH->CR2 = 0x00;
    while((!(FLASH->IAPSR & 0x08)) & (T < 0xFFFF))T++;
    if(T == 0xFFFF)return 0;
    while(Lenth--)
    {
        *EEP++ = *RxBuffer++;
        T = 0;
        while((!(FLASH->IAPSR & 0x04)) & (T < 0xFFFF))T++;
        if(T == 0xFFFF)return 0;
    }
    FLASH->IAPSR &= ~0x08;
    return 1;
}
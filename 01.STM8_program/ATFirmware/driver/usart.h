#ifndef __USART_H
#define __USART_H

#include "ebox_core.h"

void    usart_init( USART_TypeDef *USARTx, uint32_t BaudRate, USART_WordLength_TypeDef
                    USART_WordLength, USART_StopBits_TypeDef USART_StopBits,
                    USART_Parity_TypeDef USART_Parity,  USART_Mode_TypeDef USART_Mode);
void    uart1_write(uint8_t *buf, uint16_t len);
void uart1_write_string(uint8_t *buf);
void uart1_write_cstring(const uint8_t *buf);
uint8_t uart1_read();

#endif

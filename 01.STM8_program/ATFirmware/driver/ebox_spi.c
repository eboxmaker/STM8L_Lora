#include "ebox_spi.h"

void spi_config()
{
    /* Enable SPI clock */
    //CLK_PeripheralClockConfig((CLK_Peripheral_TypeDef)CLK_Peripheral_SPI1, ENABLE);
    CLK->PCKENR1 |= CLK_PCKENR1_SPI1;//

    //PB5->SCK,PB6->MOSI,PB7->MISO
    /* Set the MOSI,MISO and SCK at high level */
    GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ENABLE);

    /* SPI configuration
    SPI_Init(   SPI1,
                SPI_FirstBit_MSB,
                SPI_BaudRatePrescaler_2,
                SPI_Mode_Master,
                SPI_CPOL_Low,
                SPI_CPHA_1Edge,
                SPI_Direction_2Lines_FullDuplex,
                SPI_NSS_Soft,
                0x07);
    */

    SPI1->CR1 = (uint8_t)((uint8_t)((uint8_t)SPI_FirstBit_MSB |
                                    (uint8_t)SPI_BaudRatePrescaler_2) |
                          (uint8_t)((uint8_t)SPI_CPOL_Low |
                                    SPI_CPHA_1Edge));
    SPI1->CR2 = (uint8_t)((uint8_t)(SPI_Direction_2Lines_FullDuplex) | (uint8_t)(SPI_NSS_Soft));
    SPI1->CR1 |= (uint8_t)(SPI_Mode_Master);
    SPI1->CR2 |= (uint8_t)SPI_CR2_SSI;
    SPI1->CRCPR = (uint8_t)0x07;

    //SPI_Cmd(SPI1,ENABLE);
    SPI1->CR1 |= SPI_CR1_SPE; /* Enable the SPI peripheral*/


    /* Configure LORA_CS as Output push-pull, used as Flash Chip select */
    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
}

uint8_t spi1_transfer(uint8_t data)
{
    while ((SPI1->SR & SPI_FLAG_TXE) == RESET)
        ;
    SPI1->DR = data;
    while ((SPI1->SR & SPI_FLAG_RXNE) == RESET)
        ;
    return SPI1->DR;
}
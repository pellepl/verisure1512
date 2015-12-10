/*
 * flir.h
 *
 *  Created on: Dec 9, 2015
 *      Author: petera
 */


#ifndef FLIR_H_
#define FLIR_H_

#include "target.h"

#define SPIx                             SPI2
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI2_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_1
#define SPIx_SCK_GPIO_PORT               GPIOI
#define SPIx_SCK_AF                      GPIO_AF5_SPI2
#define SPIx_MISO_PIN                    GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_AF                     GPIO_AF5_SPI2
#define SPIx_MOSI_PIN                    GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_AF                     GPIO_AF5_SPI2
#define SPIx_CS_PIN                      GPIO_PIN_15
#define SPIx_CS_GPIO_PORT                GPIOA

/* Definition for SPIx's DMA */
#define SPIx_TX_DMA_STREAM               DMA1_Stream4
#define SPIx_RX_DMA_STREAM               DMA1_Stream3
#define SPIx_TX_DMA_CHANNEL              DMA_CHANNEL_0
#define SPIx_RX_DMA_CHANNEL              DMA_CHANNEL_0


/* Definition for SPIx's NVIC */
#define SPIx_DMA_TX_IRQn                 DMA1_Stream4_IRQn
#define SPIx_DMA_RX_IRQn                 DMA1_Stream3_IRQn

#define SPIx_DMA_TX_IRQHandler           DMA1_Stream4_IRQHandler
#define SPIx_DMA_RX_IRQHandler           DMA1_Stream3_IRQHandler

void flir_init(void);
void flir_cs_assert(bool assert);
void flir_get(u8_t *rx_buf, u16_t len);
void flir_get_blocking(u8_t *rx_buf, u16_t len);
void flir_stop(void);


#endif /* FLIR_H_ */

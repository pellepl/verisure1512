/*
 * flir.c
 *
 *  Created on: Dec 9, 2015
 *      Author: petera
 */

#include <flir.h>
#include <target.h>
#include <miniutils.h>
#include "stm32f7xx_hal.h"

SPI_HandleTypeDef flir_spi_hdl;
DMA_HandleTypeDef hdma_tx;
DMA_HandleTypeDef hdma_rx;
static volatile bool idle = TRUE;

void flir_cs_assert(bool assert) {
  HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, assert ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/**
 * @brief SPI MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 *           - DMA configuration for transmission request by peripheral
 *           - NVIC configuration for DMA interrupt request enable
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi){
  GPIO_InitTypeDef GPIO_InitStruct;

  if (hspi->Instance == SPIx) {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    SPIx_CS_GPIO_CLK_ENABLE();
    /* Enable SPI2 clock */
    SPIx_CLK_ENABLE();
    /* Enable DMA clock */
    DMAx_SPI_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /* SPI CS GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the DMA ##################################################*/
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance = SPIx_TX_DMA_STREAM;
    hdma_tx.Init.Channel = SPIx_TX_DMA_CHANNEL;
    hdma_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_tx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode = DMA_NORMAL;
    hdma_tx.Init.Priority = DMA_PRIORITY_LOW;

    HAL_DMA_Init(&hdma_tx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(hspi, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */
    hdma_rx.Instance = SPIx_RX_DMA_STREAM;

    hdma_rx.Init.Channel = SPIx_RX_DMA_CHANNEL;
    hdma_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_rx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode = DMA_NORMAL;
    hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_rx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(hspi, hdmarx, hdma_rx);

    /*##-4- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (SPI2_TX) */
    HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(SPIx_DMA_TX_IRQn);

    /* NVIC configuration for DMA transfer complete interrupt (SPI2_RX) */
    HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPIx_DMA_RX_IRQn);
  }
}

void flir_init(void) {
  flir_spi_hdl.Instance               = SPIx;
  flir_spi_hdl.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  flir_spi_hdl.Init.Direction         = SPI_DIRECTION_2LINES;
  flir_spi_hdl.Init.CLKPhase          = SPI_PHASE_1EDGE;
  flir_spi_hdl.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  flir_spi_hdl.Init.DataSize          = SPI_DATASIZE_8BIT;
  flir_spi_hdl.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  flir_spi_hdl.Init.TIMode            = SPI_TIMODE_DISABLE;
  flir_spi_hdl.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  flir_spi_hdl.Init.CRCPolynomial     = 7;
  flir_spi_hdl.Init.NSS               = SPI_NSS_SOFT;

  flir_spi_hdl.Init.Mode = SPI_MODE_MASTER;

  if (HAL_SPI_Init(&flir_spi_hdl) != HAL_OK) {
    halt("flir spi init");
  }
  print("flir spi init ok\n");
}

void flir_stop(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  HAL_SPI_DMAStop(&flir_spi_hdl);
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = 0;
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
}

void flir_unstop(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = SPIx_SCK_AF;
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
}

void flir_get(u8_t *rx_buf, u16_t len) {
  HAL_StatusTypeDef res = HAL_SPI_Receive_DMA(&flir_spi_hdl, rx_buf, len);
  if (res != HAL_OK) {
    halt("flir_read fail %i", res);
  }
}

void flir_get_blocking(u8_t *rx_buf, u16_t len) {
  idle = FALSE;
  flir_get(rx_buf, len);
  while (!idle);
}

void flir_get_blocking_hal(u8_t *rx_buf, u16_t len) {
  HAL_StatusTypeDef res = HAL_SPI_Receive(&flir_spi_hdl, rx_buf, len, 9999);
  if (res != HAL_OK) {
    halt("flir_read fail %i", res);
  }
}

/**
 * @brief  This function handles DMA Rx interrupt request.
 * @param  None
 * @retval None
 */
void SPIx_DMA_RX_IRQHandler(void) {
  HAL_DMA_IRQHandler(flir_spi_hdl.hdmarx);
}

/**
 * @brief  This function handles DMA Tx interrupt request.
 * @param  None
 * @retval None
 */
void SPIx_DMA_TX_IRQHandler(void) {
  HAL_DMA_IRQHandler(flir_spi_hdl.hdmatx);
}

/**
  * @brief  TxRx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of DMA TxRx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
//void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
  idle = TRUE;
}

/**
  * @brief  SPI error callbacks.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
  flir_cs_assert(FALSE);
  idle = TRUE;
  halt("spi error");
}


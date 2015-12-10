/*
 * joystick.c
 *
 *  Created on: Dec 10, 2015
 *      Author: petera
 */

#include <target.h>
#include <joystick.h>
#include <miniutils.h>
#include "stm32f7xx_hal.h"

static ADC_HandleTypeDef joystick_adc_hdl;

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
  GPIO_InitTypeDef GPIO_InitStruct;
  static DMA_HandleTypeDef hdma_adc;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC3 Periph clock enable */
  ADCx_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  ADCx_CHANNEL_GPIO_CLK_ENABLE();
  /* Enable DMA2 clock */
  DMAx_ADC_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* ADC Channel GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN_H;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT_H, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN_V;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT_V, &GPIO_InitStruct);

  /*##-3- Configure the DMA streams ##########################################*/
  /* Set the parameters to be configured */
  hdma_adc.Instance = ADCx_DMA_STREAM;

  hdma_adc.Init.Channel = ADCx_DMA_CHANNEL;
  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_adc.Init.Mode = DMA_CIRCULAR;
  hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

  HAL_DMA_Init(&hdma_adc);

  /* Associate the initialized DMA handle to the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
}

void joystick_init(void) {
  ADC_ChannelConfTypeDef adc_ch_config;
  joystick_adc_hdl.Instance = ADCx;

  joystick_adc_hdl.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
  joystick_adc_hdl.Init.Resolution = ADC_RESOLUTION_12B;
  joystick_adc_hdl.Init.ScanConvMode = ENABLE; /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  joystick_adc_hdl.Init.ContinuousConvMode = DISABLE; /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  joystick_adc_hdl.Init.DiscontinuousConvMode = DISABLE; /* Parameter discarded because sequencer is disabled */
  joystick_adc_hdl.Init.NbrOfDiscConversion = 0;
  joystick_adc_hdl.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Conversion start trigged at each external event */
  joystick_adc_hdl.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  joystick_adc_hdl.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  joystick_adc_hdl.Init.NbrOfConversion = 2;
  joystick_adc_hdl.Init.DMAContinuousRequests = ENABLE;
  joystick_adc_hdl.Init.EOCSelection = DISABLE;

  if (HAL_ADC_Init(&joystick_adc_hdl) != HAL_OK) {
    halt("adc_init fail");
  }

  /*##-2- Configure ADC regular channel ######################################*/
  adc_ch_config.Channel = ADCx_CHANNEL_V;
  adc_ch_config.Rank = 1;
  adc_ch_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  adc_ch_config.Offset = 0;

  if (HAL_ADC_ConfigChannel(&joystick_adc_hdl, &adc_ch_config) != HAL_OK) {
    /* Channel Configuration Error */
    halt("adc config channel v");
  }

  adc_ch_config.Channel = ADCx_CHANNEL_H;
  adc_ch_config.Rank = 2;
  adc_ch_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  if (HAL_ADC_ConfigChannel(&joystick_adc_hdl, &adc_ch_config) != HAL_OK) {
    /* Channel Configuration Error */
    halt("adc config channel h");
  }

}

static uint32_t joy_val[2];

void joystick_sample(void) {
  if (HAL_ADC_Start_DMA(&joystick_adc_hdl, (uint32_t *)joy_val, 2) != HAL_OK) {
    halt("adc start error");
  }
}

u16_t joystick_get(joystick_dir_t dir) {
  return joy_val[dir];
}

void ADCx_DMA_IRQHandler(void) {
  HAL_DMA_IRQHandler(joystick_adc_hdl.DMA_Handle);
}


/*
 * joystick.h
 *
 *  Created on: Dec 10, 2015
 *      Author: petera
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <target.h>

#define ADCx                            ADC3
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE()
#define DMAx_ADC_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

#define ADCx_CHANNEL_PIN_V              GPIO_PIN_9
#define ADCx_CHANNEL_GPIO_PORT_V        GPIOF
#define ADCx_CHANNEL_PIN_H              GPIO_PIN_8
#define ADCx_CHANNEL_GPIO_PORT_H        GPIOF

#define ADCx_CHANNEL_V                  ADC_CHANNEL_7
#define ADCx_CHANNEL_H                  ADC_CHANNEL_6

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA_CHANNEL_2
#define ADCx_DMA_STREAM                 DMA2_Stream0

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler


typedef enum joystick_dir_e {
  J_DIR_VERT = 0,
  J_DIR_HORI
} joystick_dir_t;

void joystick_init(void);
void joystick_sample(void);
u16_t joystick_get(joystick_dir_t dir);

#endif /* JOYSTICK_H_ */

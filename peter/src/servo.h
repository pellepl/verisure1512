/*
 * servo.h
 *
 *  Created on: Dec 10, 2015
 *      Author: petera
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <target.h>

#define TIMA                          TIM3
#define TIMA_CLK_ENABLE()             __HAL_RCC_TIM3_CLK_ENABLE()
#define TIMA_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMA_GPIO_PORT                GPIOB
#define TIMA_GPIO_PIN                 GPIO_PIN_4
#define TIMA_GPIO_AF                  GPIO_AF2_TIM3
#define TIMA_CHANNEL                  TIM_CHANNEL_1

#define TIMB                          TIM12
#define TIMB_CLK_ENABLE()             __HAL_RCC_TIM12_CLK_ENABLE()
#define TIMB_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define TIMB_GPIO_PORT                GPIOH
#define TIMB_GPIO_PIN                 GPIO_PIN_6
#define TIMB_GPIO_AF                  GPIO_AF9_TIM12
#define TIMB_CHANNEL                  TIM_CHANNEL_1

void servo_init(void);
void servo_set(u8_t port, u8_t pwm);

#endif /* SERVO_H_ */

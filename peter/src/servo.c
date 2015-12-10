/*
 * servo.c
 *
 *  Created on: Dec 10, 2015
 *      Author: petera
 */

#include <target.h>
#include <servo.h>
#include <miniutils.h>
#include "stm32f7xx_hal.h"

static TIM_HandleTypeDef tima_hdl;
static TIM_HandleTypeDef timb_hdl;
static TIM_OC_InitTypeDef oc_init;

void servo_init(void) {
  GPIO_InitTypeDef gpio_init;

  TIMA_GPIO_CLK_ENABLE();
  TIMA_CLK_ENABLE();

  TIMB_GPIO_CLK_ENABLE();
  TIMB_CLK_ENABLE();

  gpio_init.Pin = TIMA_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_HIGH;
  gpio_init.Alternate = TIMA_GPIO_AF;
  HAL_GPIO_Init(TIMA_GPIO_PORT, &gpio_init);

  gpio_init.Pin = TIMB_GPIO_PIN;
  gpio_init.Alternate = TIMB_GPIO_AF;
  HAL_GPIO_Init(TIMB_GPIO_PORT, &gpio_init);

  TIM_ClockConfigTypeDef clocksource_cfg;
  clocksource_cfg.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&tima_hdl, &clocksource_cfg);
  HAL_TIM_ConfigClockSource(&timb_hdl, &clocksource_cfg);

  const u32_t prescaler = (u32_t)((SystemCoreClock / 2) / (50*65536));
  print("tim pwm prescaler %i\n", prescaler);

  tima_hdl.Instance = TIMA;
  tima_hdl.Init.Period = 0xffff;
  tima_hdl.Init.Prescaler = prescaler;
  tima_hdl.Init.ClockDivision = 0;
  tima_hdl.Init.CounterMode = TIM_COUNTERMODE_UP;
  tima_hdl.Init.RepetitionCounter = 0;

  if (HAL_TIM_PWM_Init(&tima_hdl) != HAL_OK) {
    halt("tim a setup");
  }

  timb_hdl.Instance = TIMB;
  timb_hdl.Init.Period = 0xffff;
  timb_hdl.Init.Prescaler = prescaler;
  timb_hdl.Init.ClockDivision = 0;
  timb_hdl.Init.CounterMode = TIM_COUNTERMODE_UP;
  timb_hdl.Init.RepetitionCounter = 0;

  if (HAL_TIM_PWM_Init(&timb_hdl) != HAL_OK) {
    halt("tim b setup");
  }

  oc_init.OCMode = TIM_OCMODE_PWM1;
  oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
  oc_init.OCNPolarity = TIM_OCNPOLARITY_LOW;
  oc_init.OCIdleState = TIM_OCIDLESTATE_SET;
  oc_init.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  oc_init.OCFastMode = TIM_OCFAST_DISABLE;
  oc_init.Pulse = 0xffff/2;

  if (HAL_TIM_PWM_ConfigChannel(&tima_hdl, &oc_init, TIMA_CHANNEL) != HAL_OK) {
    halt("tim a pwm setup");
  }
  if (HAL_TIM_PWM_Start(&tima_hdl, TIMA_CHANNEL) != HAL_OK) {
    halt("tim a pwm start");
  }

  if (HAL_TIM_PWM_ConfigChannel(&timb_hdl, &oc_init, TIMB_CHANNEL) != HAL_OK) {
    halt("tim b pwm setup");
  }
  if (HAL_TIM_PWM_Start(&timb_hdl, TIMB_CHANNEL) != HAL_OK) {
    halt("tim b pwm start");
  }
}

void servo_set(u8_t port, u8_t pwm) {
  // servo => 1ms    --    1.5ms    --    2ms    ||   20ms
  // timer => 0xccd        0x1333         0x199a ||   0xffff
  oc_init.Pulse = ((0x199a - 0xccd) * pwm / 256) + 0xccd;
  if (HAL_TIM_PWM_ConfigChannel(
      port ? &timb_hdl : &tima_hdl,
      &oc_init,
      port ? TIMB_CHANNEL : TIMA_CHANNEL) != HAL_OK) {
    halt("tim %c pwm set", port ? 'b' : 'a');
  }
  if (HAL_TIM_PWM_Start(
      port ? &timb_hdl : &tima_hdl,
      port ? TIMB_CHANNEL : TIMA_CHANNEL) != HAL_OK) {
    halt("tim %c pwm start", port ? 'b' : 'a');
  }
}


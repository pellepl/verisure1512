/*
 * main.c
 *
 *  Created on: Dec 7, 2015
 *      Author: petera
 */

#include <target.h>

static void delay(uint32_t loops) {
  volatile uint32_t a = loops;
  while (--a);
}

int main(void) {
  GPIO_InitTypeDef gpio_init_structure;

  __HAL_RCC_GPIOI_CLK_ENABLE();

  HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1);

  gpio_init_structure.Pin       = GPIO_PIN_1;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FAST;
  //gpio_init_structure.Alternate = 0; // nevermind
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  while (1) {
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
    delay(1000000);
  }

  return 0;
}

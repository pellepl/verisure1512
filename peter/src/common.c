/*
 * common.c
 *
 *  Created on: Dec 9, 2015
 *      Author: petera
 */

#include <target.h>
#include <uart_driver.h>
#include <miniutils.h>
#include "stm32f7xx_hal.h"
#include <stdarg.h>

void halt(const char *str, ...) {
  UART_tx_flush(_UART(IOSTD));
  UART_sync_tx(_UART(IOSTD), TRUE);

  print("\n**** HALT ****\n");
  va_list arg_p;
  va_start(arg_p, str);
  vprint(str, arg_p);
  va_end(arg_p);
  print("\n");
  while (1) {
    //HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
    //HAL_Delay(500);
  }
}




/*
 * target.h
 *
 *  Created on: Dec 7, 2015
 *      Author: petera
 */

#ifndef TARGET_H_
#define TARGET_H_

#include <common.h>
#include <stm32f7xx_hal_conf.h>
#include <stm32f7xx.h>

#define FBUF_ADDR (0xc0000000)

#define LCD_W 480
#define LCD_H 272
#define LCD_WW 480
#define LCD_WH 272

#define CONFIG_UART_CNT 1
#define CONFIG_UART6


#endif /* TARGET_H_ */

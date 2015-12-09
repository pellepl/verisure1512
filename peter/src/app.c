/*
 * app.c
 *
 *  Created on: Dec 9, 2015
 *      Author: petera
 */

#include <target.h>
#include <app.h>
#include <miniutils.h>

#define LCD_W 480
#define LCD_H 272

void app_start(void) {
  memset(FBUF_ADDR, 0, LCD_W*LCD_H*2);
  u16_t *lcd = (u16_t *)FBUF_ADDR;
  int c = 1;
  int x, y;
  while (1) {
    for (y = 0; y < LCD_H; y++) {
      for (x = 0; x < LCD_W; x++) {
        lcd[x + y*LCD_W] =
            (c & 0xfffffff) > 0x8000000 ? 0xffff : 0x0000;
        c *= 0xa72e1069;
        if (c == 0) c = 1;
      }
    }
  }
}

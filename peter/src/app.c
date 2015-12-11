/*
 * app.c
 *
 *  Created on: Dec 9, 2015
 *      Author: petera
 */

#include <target.h>
#include <app.h>
#include <miniutils.h>
#include <flir.h>
#include <joystick.h>
#include <servo.h>
#include <stm32746g_discovery_ts.h>


#define FLIR_W 80
#define FLIR_H 60

#define FLIR_INVERT_Y
#define FLIR_INVERT_X

#define C565(r,g,b) ((((r)&0b00011111) << (5+6)) | (((g)&0b00111111) << 5) | (((b)&0b00011111)))

#define JOYSTICK_ADC_MIN 0x0600
#define JOYSTICK_ADC_MAX 0x0a00

#define CONTROL_JOYSTICK
#define CONTROL_JOYSTICK_RELATIVE
//#define CONTROL_TOUCHSCREEN

#define CONTROL_TOUCHSCREEN_BORDER_X  16
#define CONTROL_TOUCHSCREEN_BORDER_Y  8

//#define GRAYSCALE

#ifdef GRAYSCALE
#define COLOR_RANGE 0xff
#else
#define COLOR_RANGE (32+64+64+64+32)
#endif

#define FLIR_LCD_OFFS_X ((LCD_WW-(FLIR_W*4))/2)
#define FLIR_LCD_OFFS_Y ((LCD_WH-(FLIR_H*4))/2)


u16_t color_tbl[COLOR_RANGE];
static u8_t flir_line_pkt[4+2*FLIR_W];
static u16_t *lcd = (u16_t *)FBUF_ADDR;
static u16_t flir_max_val = 0;
static u16_t flir_min_val = 0;
static u16_t flir_max_val_prev = 0;
static u16_t flir_min_val_prev = 0;
static u16_t flir_aoi_x0, flir_aoi_x1, flir_aoi_y0, flir_aoi_y1;

#ifdef CONTROL_JOYSTICK_RELATIVE
static u8_t pan;
static u8_t tilt;
#endif
#ifndef CONTROL_TOUCHSCREEN
static struct {
  u16_t x0, y0, x1, y1;
} aoi;
static bool drag = FALSE;
static u16_t anchor_x, anchor_y;
#endif

static void flir2lcd_memcpy_line_quad(u16_t line, u16_t *src, u16_t sz) {
  if (line >= FLIR_H) return;
  u16_t fx = 0;
  u16_t lcd_line;
#ifdef FLIR_INVERT_Y
  lcd_line = FLIR_H - line - 1;
#else
  lcd_line = line;
#endif

  u16_t *dst = &lcd[lcd_line*LCD_WW*4 +
                    FLIR_LCD_OFFS_X +
                    FLIR_LCD_OFFS_Y*LCD_WW];
#ifdef FLIR_INVERT_X
  src += FLIR_W;
#endif
  while(sz--) {
    u16_t flir;
#ifdef FLIR_INVERT_X
    flir = *--src;
#else
    flir = *src++;
#endif
    flir = ((flir & 0xff00) >> 8) | ((flir & 0xff) << 8);
    if (line >= flir_aoi_y0 && line <= flir_aoi_y1 &&
        fx >= flir_aoi_x0 && fx <= flir_aoi_x1) {
      flir_min_val = MIN(flir, flir_min_val);
      flir_max_val = MAX(flir, flir_max_val);
    }

    fx++;

    u32_t f;
    u16_t diff = flir_max_val_prev - flir_min_val_prev;
    if (diff > 0) {
      // adjust to min
      u16_t flir_comp = flir < flir_min_val_prev ? 0 : (flir - flir_min_val_prev);
      // adjust to max
      flir_comp = MIN(diff, flir_comp);
      // linearize
      f = ((COLOR_RANGE-1) * flir_comp) / diff;
    } else {
      f = (COLOR_RANGE * flir) >> 14;
    }
    f &= 0xff;
    u16_t px =
#ifdef GRAYSCALE
        (((f >> 3) & 0b00011111) << (6+5)) |
        (((f >> 2) & 0b00111111) << (5))   |
        (((f >> 3) & 0b00011111) << (0))   |
        0;
#else
    color_tbl[f];
#endif

    dst[LCD_WW*3] = px;
    dst[LCD_WW*2] = px;
    dst[LCD_WW] = px;
    *dst++ = px;
    dst[LCD_WW*3] = px;
    dst[LCD_WW*2] = px;
    dst[LCD_WW] = px;
    *dst++ = px;
    dst[LCD_WW*3] = px;
    dst[LCD_WW*2] = px;
    dst[LCD_WW] = px;
    *dst++ = px;
    dst[LCD_WW*3] = px;
    dst[LCD_WW*2] = px;
    dst[LCD_WW] = px;
    *dst++ = px;
  }
}

static void draw_rect(u16_t x0, u16_t y0, u16_t x1, u16_t y1) {
  u16_t xl = MIN(x0, x1);
  u16_t xh = MAX(x0, x1);
  u16_t yl = MIN(y0, y1);
  u16_t yh = MAX(y0, y1);

  u16_t x;
  for (x = xl; x <= xh; x++) {
    lcd[yl*LCD_WW + x] = 0xffff;
    lcd[yh*LCD_WW + x] = 0xffff;
  }
//  memset(&lcd[yl*LCD_WW + xl], 0xff, (xh-xl)*2);
//  memset(&lcd[yh*LCD_WW + xl], 0xff, (xh-xl)*2);
  u16_t y;
  for (y = yl; y <= yh; y++) {
    lcd[y*LCD_WW + xl] = 0xffff;
    lcd[y*LCD_WW + xh] = 0xffff;
  }
}

static void fill_color_tbl(void) {
  int i;
  int ix = 0;
  // blue
  for (i = 0; i < 32; i++) {
    color_tbl[ix++] = C565(0,0,i);
  }
  // cyan
  for (i = 0; i < 64; i++) {
    color_tbl[ix++] = C565(0,i,31);
  }
  // magenta
  for (i = 0; i < 64; i++) {
    color_tbl[ix++] = C565(i/2,63-i,31);
  }
  // yellow
  for (i = 0; i < 64; i++) {
    color_tbl[ix++] = C565(31,i,31-i/2);
  }
  // white
  for (i = 0; i < 32; i++) {
    color_tbl[ix++] = C565(31,63,i);
  }
}

static void draw_flir_range(void) {
  const u16_t margin = 16;
  u16_t *disp = &lcd[LCD_WW*(LCD_WH-6) + margin];
  u16_t bar_range = LCD_WW - margin * 2;

  u32_t area_min = flir_min_val_prev * bar_range / (1<<14);
  u32_t area_max = flir_max_val_prev * bar_range / (1<<14);

  u16_t x;
  for (x = 0; x < bar_range; x++) {
    u16_t col =
        (x >= area_min && x <= area_max) ? C565(31,63,31) : C565(15,31,15);
    disp[LCD_WW * 3] = col;
    disp[LCD_WW * 2] = col;
    disp[LCD_WW * 1] = col;
    *disp++ = col;
  }
}

static u8_t normalize_adc_readout(u16_t x) {
  u32_t v = MAX(JOYSTICK_ADC_MIN, MIN(JOYSTICK_ADC_MAX, x));
  v = ((v - JOYSTICK_ADC_MIN) * 0xff) / (JOYSTICK_ADC_MAX - JOYSTICK_ADC_MIN);
  return v;
}

static void joystick_readout(void) {
  u16_t joy_v = joystick_get(J_DIR_VERT);
  u16_t joy_h = joystick_get(J_DIR_HORI);

  // normalize to 8 bit
  joy_v = normalize_adc_readout(joy_v);
  joy_h = normalize_adc_readout(joy_h);

#ifdef CONTROL_JOYSTICK
#ifdef CONTROL_JOYSTICK_RELATIVE
  if (joy_h < 0x70 && pan > 0x00) pan--;
  if (joy_h > 0x90 && pan < 0xff) pan++;
  if (joy_v < 0x70 && tilt > 0x00) tilt--;
  if (joy_v > 0x90 && tilt < 0xff) tilt++;
  servo_set(0, pan);
  servo_set(1, tilt);
#else
  servo_set(0, joy_h);
  servo_set(1, joy_v);
#endif
#endif

  // visualize
  u16_t *disp = lcd;
  u32_t y;
  for (y = 0; y < 128; y++) {
    u16_t v_col = ((joy_v < 128) & (joy_v <= y)) ? C565(0,63,0) : C565(8,16,8);
    u16_t h_col = ((joy_h < 128) & (joy_h <= y)) ? C565(31,0,0) : C565(8,16,8);
    disp[y*LCD_WW] = v_col;
    disp[y*LCD_WW+1] = v_col;
    disp[y*LCD_WW+4] = h_col;
    disp[y*LCD_WW+5] = h_col;
  }
  for (y = 128; y < 256; y++) {
    u16_t v_col = ((joy_v >= 128) & (joy_v >= y)) ? C565(0,63,0) : C565(8,16,8);
    u16_t h_col = ((joy_h >= 128) & (joy_h >= y)) ? C565(31,0,0) : C565(8,16,8);
    disp[y*LCD_WW] = v_col;
    disp[y*LCD_WW+1] = v_col;
    disp[y*LCD_WW+4] = h_col;
    disp[y*LCD_WW+5] = h_col;
  }

  // start a new conversion
  joystick_sample();
}

static u16_t lcd2flir_x(u16_t lcd_x) {
  return (lcd_x - FLIR_LCD_OFFS_X) / 4;
}
static u16_t lcd2flir_y(u16_t lcd_y) {
  return (lcd_y - FLIR_LCD_OFFS_Y) / 4;
}

static void touchscreen_readout(void) {
  TS_StateTypeDef ts;
  u8_t res = BSP_TS_GetState(&ts);
  if (res != TS_OK) halt("ts getstate");
#ifdef CONTROL_TOUCHSCREEN
  if (ts.touchDetected > 0) {
    s32_t x = ts.touchX[0];
    s32_t y = ts.touchY[0];
    x -= CONTROL_TOUCHSCREEN_BORDER_X;
    y -= CONTROL_TOUCHSCREEN_BORDER_Y;
    x = MAX(0, MIN(0xff, x));
    y = MAX(0, MIN(0xff, y));
    servo_set(0, y);
    servo_set(1, 255-x);
  }
#else
  if (ts.touchDetected == 1) {
    u16_t x = ts.touchX[0];
    u16_t y = ts.touchY[0];
    if (x < aoi.x0 || x > aoi.x1 || y < aoi.y0 || y > aoi.y1) {
      // outside selection
      memset(&aoi, 0, sizeof(aoi));
      drag = FALSE;
    } else {
      // inside selection
      if (!drag) {
        drag = TRUE;
        anchor_x = x;
        anchor_y = y;
      } else {
        s32_t dx = x - anchor_x;
        s32_t dy = y - anchor_y;
        anchor_x = x;
        anchor_y = y;
        if (aoi.x0 + dx < FLIR_LCD_OFFS_X) {
          dx = FLIR_LCD_OFFS_X - aoi.x0;
        } else if (aoi.x1 + dx > FLIR_LCD_OFFS_X + FLIR_W*4 - 1) {
          dx = FLIR_LCD_OFFS_X + FLIR_W*4 - 1 - aoi.x1;
        }
        if (aoi.y0 + dy < FLIR_LCD_OFFS_Y) {
          dy = FLIR_LCD_OFFS_Y - aoi.y0;
        } else if (aoi.y1 + dy > FLIR_LCD_OFFS_Y + FLIR_H*4 - 1) {
          dy = FLIR_LCD_OFFS_Y + FLIR_H*4 - 1 - aoi.y1;
        }
        aoi.x0 += dx;
        aoi.x1 += dx;
        aoi.y0 += dy;
        aoi.y1 += dy;
      }
    }
  } else if (ts.touchDetected == 2) {
    aoi.x0 = MIN(ts.touchX[0], ts.touchX[1]);
    aoi.x1 = MAX(ts.touchX[0], ts.touchX[1]);
    aoi.y0 = MIN(ts.touchY[0], ts.touchY[1]);
    aoi.y1 = MAX(ts.touchY[0], ts.touchY[1]);
    aoi.x0 = MIN(FLIR_LCD_OFFS_X + FLIR_W*4 - 1 , MAX(FLIR_LCD_OFFS_X, aoi.x0));
    aoi.x1 = MIN(FLIR_LCD_OFFS_X + FLIR_W*4 - 1 , MAX(FLIR_LCD_OFFS_X, aoi.x1));
    aoi.y0 = MIN(FLIR_LCD_OFFS_Y + FLIR_H*4 - 1, MAX(FLIR_LCD_OFFS_Y, aoi.y0));
    aoi.y1 = MIN(FLIR_LCD_OFFS_Y + FLIR_H*4 - 1, MAX(FLIR_LCD_OFFS_Y, aoi.y1));
    drag = FALSE;
  } else if (ts.touchDetected == 0) {
    drag = FALSE;
  }
  if (aoi.x0 != aoi.x1 && aoi.y0 != aoi.y1) {
    draw_rect(aoi.x0, aoi.y0, aoi.x1, aoi.y1);
  }
#endif
}

static void on_vsync() {
  draw_flir_range();
  joystick_readout();
  touchscreen_readout();
}

void app_start(void) {
  servo_init();
  servo_set(0, 128);
  servo_set(1, 128);
#ifdef CONTROL_JOYSTICK_RELATIVE
  pan = 128;
  tilt = 128;
#endif

  flir_init();
  fill_color_tbl();
  joystick_init();
  if (
#ifdef CONTROL_TOUCHSCREEN
      BSP_TS_Init(255 + CONTROL_TOUCHSCREEN_BORDER_X*2, 255 + CONTROL_TOUCHSCREEN_BORDER_Y*2)
#else
      BSP_TS_Init(LCD_WW, LCD_WH)
#endif
      != TS_OK) halt("ts init");

  print("reading flir\n");
  joystick_sample();
  while (1) {
    memset(lcd, 0, LCD_WW*LCD_WH*2);
    memset(flir_line_pkt, 0, sizeof(flir_line_pkt));
    flir_cs_assert(TRUE);
    bool vsync = FALSE;
    u16_t max_line = 0;
    u16_t prev_line = 0;
    flir_aoi_x0 = 0;
    flir_aoi_x1 = FLIR_W;
    flir_aoi_y0 = 0;
    flir_aoi_y1 = FLIR_H;
    while (1) {
      flir_get_blocking(flir_line_pkt, sizeof(flir_line_pkt));
      if (flir_line_pkt[1] != 0xff) {
        vsync = FALSE;
        u16_t line = flir_line_pkt[1];
        if (prev_line != 0 && line != prev_line + 1) {
          print("UNSYNCED, prev_line:%i line:%i\n", prev_line, line);
          break;
        }
        max_line = MAX(max_line, line);
        prev_line = line;
        flir2lcd_memcpy_line_quad(line, (u16_t*)&flir_line_pkt[4], FLIR_W);
      } else {
        if (!vsync) {
          vsync = TRUE;
#ifndef CONTROL_TOUCHSCREEN
          if (aoi.x0 != aoi.x1 && aoi.y0 != aoi.y1) {
            flir_aoi_x0 = lcd2flir_x(MIN(aoi.x0, aoi.x1));
            flir_aoi_x1 = lcd2flir_x(MAX(aoi.x0, aoi.x1));
#ifndef FLIR_INVERT_Y
            flir_aoi_y0 = lcd2flir_y(MIN(aoi.y0, aoi.y1));
            flir_aoi_y1 = lcd2flir_y(MAX(aoi.y0, aoi.y1));
#else
            flir_aoi_y1 = FLIR_H - lcd2flir_y(MIN(aoi.y0, aoi.y1)) - 1;
            flir_aoi_y0 = FLIR_H - lcd2flir_y(MAX(aoi.y0, aoi.y1)) - 1;
#endif
          } else {
            flir_aoi_x0 = 0;
            flir_aoi_x1 = FLIR_W;
            flir_aoi_y0 = 0;
            flir_aoi_y1 = FLIR_H;
          }
#endif
//          print("VSYNC, max_line %i, min-max:%04x-%04x\n",
//              max_line, flir_min_val, flir_max_val);
          max_line = 0;
          prev_line = 0;
          flir_min_val_prev = flir_min_val;
          flir_max_val_prev = flir_max_val;
          flir_min_val = 0xffff;
          flir_max_val = 0x0000;
          on_vsync();
        }
      }
    }

    print("resyncing...\n");
    flir_stop();
    flir_cs_assert(TRUE);

    int c = 1;
    int x, y;
    volatile u32_t a = 0x40;
    while (a--) {
      for (y = 0; y < LCD_WH; y++) {
        for (x = 0; x < LCD_WW; x++) {
          lcd[x + y*LCD_WW] =
              (c & 0xfffffff) > 0x8000000 ? 0xffff : 0x0000;
          c *= 0xa72e1069;
          if (c == 0) c = 1;
        }
      }
    }

    flir_unstop();
  } // superloop

}

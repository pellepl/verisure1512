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

#define FLIR_W 80
#define FLIR_H 60

#define C565(r,g,b) ((((r)&0b00011111) << (5+6)) | (((g)&0b00111111) << 5) | (((b)&0b00011111)))

#define ADC_MIN 0x0600
#define ADC_MAX 0x0a00

//#define GRAYSCALE

#ifdef GRAYSCALE
#define RANGE 0xff
#else
#define RANGE (32+64+64+64+32)
#endif


u16_t color_tbl[RANGE];
static u8_t flir_line_pkt[4+2*FLIR_W];
static u16_t *lcd = (u16_t *)FBUF_ADDR;
static u16_t flir_max_val = 0;
static u16_t flir_min_val = 0;
static u16_t flir_max_val_prev = 0;
static u16_t flir_min_val_prev = 0;

static void flir2lcd_memcpy_quad(u16_t line, u16_t *src, u16_t sz) {
  u16_t *dst = &lcd[line*LCD_WW*4 +
                    (LCD_WW-(FLIR_W*4))/2 +
                    ((LCD_WH-(FLIR_H*4))/2)*LCD_WW];
  while(sz--) {
    u16_t flir = *src++;
    flir = ((flir & 0xff00) >> 8) | ((flir & 0xff) << 8);
    flir_min_val = MIN(flir, flir_min_val);
    flir_max_val = MAX(flir, flir_max_val);

    u32_t f;
    u16_t diff = flir_max_val_prev - flir_min_val_prev;
    if (diff > 0) {
      // adjust to min
      u16_t flir_comp = flir < flir_min_val_prev ? 0 : (flir - flir_min_val_prev);
      // adjust to max
      flir_comp = MIN(diff, flir_comp);
      // linearize
      f = ((RANGE-1) * flir_comp) / diff;
    } else {
      f = (RANGE * flir) >> 14;
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


static u8_t normalize_adc_readout(u16_t x) {
  u32_t v = MAX(ADC_MIN, MIN(ADC_MAX, x));
  v = ((v - ADC_MIN) * 0xff) / (ADC_MAX - ADC_MIN);
  return v;
}

static void joystick_readout(void) {
  u16_t joy_v = joystick_get(J_DIR_VERT);
  u16_t joy_h = joystick_get(J_DIR_HORI);

  // normalize to 8 bit
  joy_v = normalize_adc_readout(joy_v);
  joy_h = normalize_adc_readout(joy_h);

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

static void on_vsync() {
  joystick_readout();
}

void app_start(void) {
  flir_init();
  fill_color_tbl();
  joystick_init();

  print("reading flir\n");
  joystick_sample();
  while (1) {
    memset(lcd, 0, LCD_WW*LCD_WH*2);
    memset(flir_line_pkt, 0, sizeof(flir_line_pkt));
    flir_cs_assert(TRUE);
    bool vsync = FALSE;
    u16_t max_line = 0;
    u16_t prev_line = 0;
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
        flir2lcd_memcpy_quad(line, (u16_t*)&flir_line_pkt[4], FLIR_W);
      } else {
        if (!vsync) {
          vsync = TRUE;
          print("VSYNC, max_line %i, min-max:%04x-%04x\n",
              max_line, flir_min_val, flir_max_val);
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
    flir_cs_assert(FALSE);

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

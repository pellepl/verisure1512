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

#define LEPTON_CHECK_CRC
#ifdef LEPTON_CHECK_CRC
#include <lepton_packet.h>
#endif

#define FLIR_W 80
#define FLIR_H 60

#define FLIR_INVERT_Y
#define FLIR_INVERT_X

#define C565(r,g,b) ((((r)&0b00011111) << (5+6)) | (((g)&0b00111111) << 5) | (((b)&0b00011111)))

#define JOYSTICK_ADC_MIN 0x0600
#define JOYSTICK_ADC_MAX 0x0a00

//#define CONTROL_JOYSTICK
#define CONTROL_TOUCHSCREEN

#define CONTROL_TOUCHSCREEN_BORDER_X  16
#define CONTROL_TOUCHSCREEN_BORDER_Y  8

//#define GRAYSCALE
//#define TEMP_RANGE_DYNAMIC
// About -18 degree C (calibrated on fridge -18 C)
#define TEMP_RANGE_STATIC_MIN  (0x1B00)
// About +100 degree C (calibrated on boiling water +100 C)
#define TEMP_RANGE_STATIC_MAX  (0x2A00)

#ifdef GRAYSCALE
#define COLOR_RANGE 0xff
#else
#define COLOR_RANGE (32+64+64+64+32)
#endif


u16_t color_tbl[COLOR_RANGE];
static u8_t flir_line_pkt[4+2*FLIR_W];
static u16_t *lcd = (u16_t *)FBUF_ADDR;
static u16_t flir_max_val = 0;
static u16_t flir_min_val = 0;
static u16_t flir_max_val_prev = 0;
static u16_t flir_min_val_prev = 0;

static void flir2lcd_memcpy_quad(u16_t line, u16_t *src, u16_t sz) {
  //  u16_t *dst = &lcd[line*LCD_WW*4 +
  u16_t lcd_line;
#ifdef FLIR_INVERT_Y
  lcd_line = FLIR_H - line;
#else
  lcd_line = line;
#endif

  u16_t *dst = &lcd[lcd_line*LCD_WW*4 +
                    (LCD_WW-(FLIR_W*4))/2 +
                    ((LCD_WH-(FLIR_H*4))/2)*LCD_WW];
#ifdef FLIR_INVERT_X
  src += FLIR_W;
#endif
  while(sz--) {
    //    u16_t flir = *src++;
    u16_t flir;
#ifdef FLIR_INVERT_X
    flir = *--src;
#else
    flir = *src++;
#endif
    flir = ((flir & 0xff00) >> 8) | ((flir & 0xff) << 8);

#ifdef TEMP_RANGE_DYNAMIC
    // dynamic range
    flir_min_val = MIN(flir, flir_min_val);
    flir_max_val = MAX(flir, flir_max_val);
#endif

    u32_t f;
    u16_t range = flir_max_val_prev - flir_min_val_prev;
    if (range > 0) {
      // adjust to min
      u16_t flir_comp = (flir < flir_min_val_prev) ? 0 : (flir - flir_min_val_prev);
      // adjust to max
      flir_comp = MIN(range, flir_comp);
      // linearize
      f = ((COLOR_RANGE-1) * flir_comp) / range;
    } else {
      f = (COLOR_RANGE * flir) >> 14;
    }
    f &= 0xff;

    // transform to colormap
    u16_t px =
#ifdef GRAYSCALE
        (((f >> 3) & 0b00011111) << (6+5)) |
        (((f >> 2) & 0b00111111) << (5))   |
        (((f >> 3) & 0b00011111) << (0))   |
        0;
#else
    color_tbl[f];
#endif

    // quad pixel
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
  servo_set(0, joy_h);
  servo_set(1, joy_v);
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
  u8_t i;
  for (i = 0; i < ts.touchDetected; i++) {
    print("touch%i @ %i,%i\n", i, ts.touchX[i], ts.touchY[i]);
  }
#endif
}

static void on_vsync() {
  joystick_readout();
  touchscreen_readout();
}

void app_start(void) {
  servo_init();
  servo_set(0, 128);
  servo_set(1, 128);

  flir_init();
  fill_color_tbl();
  joystick_init();
  //  if (BSP_TS_Init(LCD_WW, LCD_WH) != TS_OK) halt("ts init");
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
    while (1) {
      flir_get_blocking(flir_line_pkt, sizeof(flir_line_pkt));
      if (flir_line_pkt[1] != 0xff) {
        vsync = FALSE;

#ifdef LEPTON_CHECK_CRC
        int crc_ok = lepton_packet_check_crc((struct lepton_packet_s *)flir_line_pkt);
        if (! crc_ok) {
          print("CRCERR\n");
        }
#endif

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
#if 0
          print("VSYNC, max_line %i, min-max:%04x-%04x\n",
              max_line, flir_min_val, flir_max_val);
#endif
          max_line = 0;
          prev_line = 0;

          flir_min_val_prev = flir_min_val;
          flir_max_val_prev = flir_max_val;
#ifdef TEMP_RANGE_DYNAMIC
          flir_min_val = 0xffff;
          flir_max_val = 0x0000;
#else
          flir_min_val = TEMP_RANGE_STATIC_MIN;
          flir_max_val = TEMP_RANGE_STATIC_MAX;
#endif
          on_vsync();
        }
      }
    }

    print("resyncing...\n");
    flir_stop();
#ifdef FLIR_PULL_CS_ON_RESYNC
    flir_cs_assert(FALSE);
#endif

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

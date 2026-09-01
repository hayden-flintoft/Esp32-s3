// Copy this file to Arduino/libraries/lv_conf.h on your phone/PC
// (a sibling of the "lvgl" library folder, NOT inside it) — see ARDUINODROID.md.
/* Minimal LVGL 8.3 configuration for the ESP32-S3 virtual pet project. */
#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   1   // TFT_eSPI/ILI9341 over SPI expects byte-swapped RGB565

#define LV_MEM_CUSTOM      0
#define LV_MEM_SIZE        (64U * 1024U)

#define LV_DISP_DEF_REFR_PERIOD 30
#define LV_INDEV_DEF_READ_PERIOD 30

#define LV_TICK_CUSTOM     1
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR  0

#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_28 1
#define LV_THEME_DEFAULT_INIT lv_theme_default_init
#define LV_USE_THEME_DEFAULT 1

#define LV_USE_LABEL   1
#define LV_USE_BTN     1
#define LV_USE_IMG     1
#define LV_USE_BAR     1
#define LV_USE_ARC     1
#define LV_USE_LINE    1
#define LV_USE_ANIMIMG 1

#define LV_USE_LOG 0
#define LV_USE_ASSERT_NULL 1
#define LV_USE_ASSERT_MALLOC 1

#endif /*LV_CONF_H*/

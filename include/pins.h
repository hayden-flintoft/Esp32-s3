#pragma once
// Pin map for the ESP32-S3 2.8" capacitive touch display board (SKU ES3C28P).
// Source: https://github.com/ngttai/esp32_s3_es3c28p (community BSP for this exact board).
// Verify against your board silkscreen before flashing -- clone variants exist.

// --- LCD (ILI9341V, 4-wire SPI) ---
#define PIN_LCD_CS    10
#define PIN_LCD_DC    46
#define PIN_LCD_SCK   12
#define PIN_LCD_MOSI  11
#define PIN_LCD_MISO  13
#define PIN_LCD_BL    45   // backlight, active HIGH
// LCD reset is tied to the ESP32-S3's own reset line on this board (no separate GPIO).

// --- Capacitive touch (FT6336G, I2C) ---
#define PIN_TOUCH_SDA 16
#define PIN_TOUCH_SCL 15
#define PIN_TOUCH_RST 18
#define PIN_TOUCH_INT 17
#define FT6336_I2C_ADDR 0x38

// --- RGB status LED (WS2812B, single pixel) ---
#define PIN_RGB_LED   42

// --- Battery voltage sense (resistor divider into ADC) ---
#define PIN_BATT_ADC  9

// --- Boot button (also usable as a user button after boot) ---
#define PIN_BOOT_BTN  0

// --- microSD (SDIO 4-bit) ---
#define PIN_SD_CLK    38
#define PIN_SD_CMD    40
#define PIN_SD_D0     39
#define PIN_SD_D1     41
#define PIN_SD_D2     48
#define PIN_SD_D3     47

// --- Audio (I2S to ES8311 codec + FM8002E amplifier) ---
#define PIN_AMP_EN    1    // active LOW enable for speaker amplifier
#define PIN_I2S_MCLK  4
#define PIN_I2S_BCLK  5
#define PIN_I2S_LRCK  7
#define PIN_I2S_DOUT  6    // ESP32-S3 -> codec (speaker path)
#define PIN_I2S_DIN   8    // codec -> ESP32-S3 (mic path, unused here)
#define ES8311_I2C_ADDR 0x18

// --- Debug UART (separate from the USB-C programming port) ---
#define PIN_DEBUG_TX  44
#define PIN_DEBUG_RX  43

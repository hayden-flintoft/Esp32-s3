// Paste this whole file's content over TFT_eSPI/User_Setup.h (see ARDUINODROID.md).
// Pin values match include/pins.h / platformio.ini for the ESP32-S3 ES3C28P board.

#define USER_SETUP_INFO "ESP32S3_ES3C28P"

#define ILI9341_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   10
#define TFT_DC   46
#define TFT_RST  -1   // reset is tied to the ESP32-S3's own reset line on this board
#define TFT_BL   45
#define TFT_BACKLIGHT_ON HIGH

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <Adafruit_NeoPixel.h>

#include "pins.h"
#include "ft6336.h"
#include "pet.h"
#include "ui.h"

static TFT_eSPI tft = TFT_eSPI();
static FT6336 touch;
static Adafruit_NeoPixel rgb(1, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);
static Pet pet;

static const uint16_t SCREEN_W = 240;
static const uint16_t SCREEN_H = 320;

// LVGL draw buffer: one 40-row slice is plenty of RAM and still fast enough.
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[SCREEN_W * 40];

static uint32_t lastLoopMs = 0;
static uint32_t lastSaveMs = 0;
static const uint32_t SAVE_INTERVAL_MS = 15000;

static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    // LV_COLOR_16_SWAP=1 in lv_conf.h already byte-swaps pixels, so don't swap again here.
    tft.pushColors((uint16_t *)color_p, w * h, false);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

static void touch_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    int16_t x, y;
    if (touch.read(x, y)) {
        // Adjust these swaps/inversions if the touch mapping is off for your panel orientation.
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static void update_mood_led() {
    PetMood m = pet.mood();
    uint8_t r = 0, g = 0, b = 0;
    switch (m) {
        case PetMood::Great: g = 40; break;
        case PetMood::Okay:  r = 30; g = 30; break;
        case PetMood::Sad:   r = 40; g = 15; break;
        case PetMood::Sick:  r = 40; break;
    }
    if (pet.state().asleep) { r /= 4; g /= 4; b /= 4; } // dim while sleeping
    rgb.setPixelColor(0, rgb.Color(r, g, b));
    rgb.show();
}

void setup() {
    Serial.begin(115200);

    pinMode(PIN_AMP_EN, OUTPUT);
    digitalWrite(PIN_AMP_EN, HIGH); // amplifier disabled (active low); no audio in this build yet

    rgb.begin();
    rgb.setBrightness(60);
    rgb.show();

    tft.init();
    tft.setRotation(0); // portrait, matches 240x320; try 2 if the image is upside down
    tft.fillScreen(TFT_BLACK);

    touch.begin(PIN_TOUCH_SDA, PIN_TOUCH_SCL, PIN_TOUCH_RST, PIN_TOUCH_INT, FT6336_I2C_ADDR);

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, SCREEN_W * 40);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_W;
    disp_drv.ver_res = SCREEN_H;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    lv_indev_drv_register(&indev_drv);

    pet.begin();
    ui::build(&pet);
    ui::refresh();
    update_mood_led();

    lastLoopMs = millis();
    lastSaveMs = millis();
}

void loop() {
    uint32_t now = millis();
    uint32_t delta = now - lastLoopMs;
    lastLoopMs = now;

    pet.update(delta);
    ui::refresh();
    update_mood_led();

    if (now - lastSaveMs >= SAVE_INTERVAL_MS) {
        pet.save();
        lastSaveMs = now;
    }

    lv_timer_handler();
    delay(5);
}

# ESP32-S3 Virtual Pet

A Tamagotchi-style virtual pet for the **ESP32-S3 2.8" capacitive touch display board** (SKU `ES3C28P`, 240x320 IPS, ILI9341 driver, FT6336G capacitive touch).

Feed, play with, clean, and put your pet to sleep by tapping the touchscreen. It hatches from an egg, grows through baby/child/adult stages, and its mood is mirrored by the onboard RGB LED. State is saved to flash (NVS) every 15s so it survives a reset/reflash.

## Hardware

Pin mapping ([`include/pins.h`](include/pins.h)) is sourced from the [community BSP for this exact board](https://github.com/ngttai/esp32_s3_es3c28p). If your board is a clone with different silkscreen labels, verify before flashing.

| Peripheral | Chip |
|---|---|
| Display | ILI9341V, 240x320, 4-wire SPI |
| Touch | FT6336G, I2C, capacitive |
| Audio | ES8311 codec + FM8002E amp (I2S) |
| RGB LED | WS2812B (single pixel) |
| Storage | microSD (SDIO 4-bit) |

## Build & flash

Requires [PlatformIO](https://platformio.org/) (VS Code extension or CLI). This project was written and reviewed carefully but **not build-tested on real hardware** — there was no ESP32 toolchain or physical board available in the environment this was authored in. Expect to fix a small thing or two on first build.

```bash
pio run -e esp32-s3-es3c28p -t upload
pio device monitor
```

If `pio run` fails to fetch libraries, check your network/proxy — it needs to reach the PlatformIO registry for `TFT_eSPI`, `lvgl`, and `Adafruit NeoPixel`.

## What's implemented

- Display + LVGL UI: pet face (color/expression reflects mood), stat bars (food/mood/rest/clean), action buttons.
- Capacitive touch via a small hand-rolled FT6336G driver (`src/ft6336.*`) — no external touch library dependency.
- Pet simulation (`src/pet.*`): hunger/happiness/energy/hygiene decay over time, sleep cycle, sickness from neglect, stage growth (egg → baby → child → adult), persisted to NVS via `Preferences`.
- RGB status LED reflects current mood.

## Known gaps / next steps

- **Audio is stubbed out.** The board's ES8311 codec + I2S speaker path needs its own bring-up (I2C register init sequence + I2S DMA) which wasn't safe to hand-write untested. `PIN_AMP_EN` is held disabled in `setup()`. A good next step is pulling in `pschatzmann/arduino-audio-tools`, which has ES8311 support, and adding a couple of short tone/chime effects for feed/play/sick.
- **Time only advances while powered on.** There's no RTC battery backup, so `ageSeconds` and stat decay don't account for time spent switched off. If you want "the pet ages while you're away," add Wi-Fi + NTP sync and compare against a stored last-seen timestamp.
- **Touch calibration/orientation** in `touch_read()` (`src/main.cpp`) is a straight passthrough of raw FT6336 coordinates. If tapping feels offset or rotated relative to what's on screen, that's the place to add axis swap/flip logic — it depends on how the touch panel is mechanically bonded to this particular display, which varies by batch.
- microSD and the mic input are wired in `pins.h` but unused — natural extensions (save high scores, custom sprites, voice-triggered feeding, etc.).

## Project layout

```
platformio.ini       PlatformIO env + TFT_eSPI pin config (via build_flags)
include/pins.h        Board pin map
include/lv_conf.h     LVGL configuration
src/main.cpp          Setup/loop, display+touch+LED init
src/ft6336.*           FT6336G capacitive touch driver
src/pet.*               Pet state machine (hardware-independent)
src/ui.*                 LVGL screen construction + refresh
```

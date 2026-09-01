# Deploying from your phone with ArduinoDroid

This project normally builds with PlatformIO (`platformio.ini`), which auto-configures TFT_eSPI's pins and LVGL's settings via `build_flags`. ArduinoDroid doesn't have that mechanism, so those two configs need to be dropped into the library folders by hand, once. Everything else is copy-paste.

Use the **`arduino/ESP32S3VirtualPet/`** folder in this repo — it's a flat copy of the same firmware, already in the `.ino` + tabs layout ArduinoDroid expects (it can't use the `src/`/`include/` split PlatformIO uses).

## 1. Get the ESP32 board support installed

In ArduinoDroid: **Menu → Board Manager** → add the ESP32 boards package if it's not already there (search "esp32" by Espressif). Then set the board to **ESP32S3 Dev Module** (Menu → Board), with:
- USB CDC On Boot: **Enabled** (needed for Serial over the USB-C port)
- Flash Size: **16MB**
- PSRAM: **OPI PSRAM**
- Partition Scheme: a 16MB scheme, e.g. "16M Flash (3MB APP/9.9MB FATFS)" or similar — exact wording varies by core version

## 2. Install the libraries

Via ArduinoDroid's Library Manager (or by downloading the GitHub zip and using "Import .zip Library" if a given library isn't listed):
- **TFT_eSPI** (bodmer)
- **lvgl** (v8.3.x — not v9, the project's `lv_conf.h` is written for v8)
- **Adafruit NeoPixel**

## 3. Configure TFT_eSPI's pins

TFT_eSPI reads its pin config from a file *inside its own library folder*, not from your sketch. Using your phone's file manager (or ArduinoDroid's built-in one):

1. Find `TFT_eSPI/User_Setup.h` under wherever ArduinoDroid keeps libraries (typically `Arduino/libraries/TFT_eSPI/User_Setup.h`).
2. Open `arduino/TFT_eSPI_User_Setup.h` from this repo, select all, copy.
3. Paste it over the *entire contents* of the library's `User_Setup.h`, replacing what's there. Save.

## 4. Configure LVGL

1. Copy `arduino/lv_conf.h` from this repo into `Arduino/libraries/lv_conf.h` — as a **sibling** of the `lvgl` folder, not inside it (i.e. `Arduino/libraries/lv_conf.h` and `Arduino/libraries/lvgl/` sit next to each other).
2. LVGL's `lvgl.h` picks this up automatically at compile time; no further edits needed.

## 5. Copy the sketch onto your phone

Copy the whole `arduino/ESP32S3VirtualPet/` folder into ArduinoDroid's sketches directory (e.g. `Arduino/ESP32S3VirtualPet/`), so the `.ino` filename matches its parent folder name — ArduinoDroid, like desktop Arduino IDE, requires that. Open `ESP32S3VirtualPet.ino` in ArduinoDroid; the other `.h`/`.cpp` files will show up as tabs automatically.

## 6. Flash it

1. Connect the board to your phone with a USB-C **OTG-capable** cable (data cable, not charge-only).
2. Grant ArduinoDroid USB permission when prompted.
3. Hit compile/upload. ESP32-S3's auto-reset-into-bootloader doesn't always work reliably over phone OTG — if the upload times out waiting for the chip, hold **BOOT**, tap **RESET** once, release **BOOT**, then retry the upload.
4. First compile will be slow (LVGL + TFT_eSPI are large) — expect several minutes on a phone.

## What to expect

The screen should light up with the pet's face, stat bars, and buttons within a few seconds of boot. If touch feels offset from what's drawn, that's a calibration/orientation tweak in `touch_read()` in the `.ino` — see the "Known gaps" section of the main [README](README.md).

This hasn't been build-tested on real hardware (no ESP32 toolchain or device in the environment this was written in), so budget time for a couple of iterations getting the first successful compile.

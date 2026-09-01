#pragma once
#include <Arduino.h>

// Minimal driver for the FT6336G capacitive touch controller (I2C).
// Reads only the primary touch point -- plenty for a single-finger UI.
class FT6336 {
public:
    void begin(int sda, int scl, int rstPin, int intPin, uint8_t addr = 0x38);

    // Returns true if a finger is currently down; fills x/y in panel pixel coords.
    bool read(int16_t &x, int16_t &y);

private:
    uint8_t _addr = 0x38;
    int _rstPin = -1;
    int _intPin = -1;

    uint8_t readReg(uint8_t reg);
};

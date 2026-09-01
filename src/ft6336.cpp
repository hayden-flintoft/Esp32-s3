#include "ft6336.h"
#include <Wire.h>

// FT6336 register map (subset)
static const uint8_t REG_TD_STATUS = 0x02; // number of touch points (0 or 1 here)
static const uint8_t REG_P1_XH     = 0x03; // bits [3:0] = X high; bits [7:6] = event flag
static const uint8_t REG_P1_XL     = 0x04;
static const uint8_t REG_P1_YH     = 0x05;
static const uint8_t REG_P1_YL     = 0x06;

void FT6336::begin(int sda, int scl, int rstPin, int intPin, uint8_t addr) {
    _addr = addr;
    _rstPin = rstPin;
    _intPin = intPin;

    Wire.begin(sda, scl);
    Wire.setClock(400000);

    if (_rstPin >= 0) {
        pinMode(_rstPin, OUTPUT);
        digitalWrite(_rstPin, LOW);
        delay(10);
        digitalWrite(_rstPin, HIGH);
        delay(50); // FT6336 needs time to boot after reset
    }
    if (_intPin >= 0) {
        pinMode(_intPin, INPUT); // polled, not interrupt-driven
    }
}

uint8_t FT6336::readReg(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        return 0;
    }
    Wire.requestFrom((int)_addr, 1);
    if (Wire.available() < 1) {
        return 0;
    }
    return Wire.read();
}

bool FT6336::read(int16_t &x, int16_t &y) {
    uint8_t touches = readReg(REG_TD_STATUS) & 0x0F;
    if (touches == 0) {
        return false;
    }

    uint8_t xh = readReg(REG_P1_XH);
    uint8_t xl = readReg(REG_P1_XL);
    uint8_t yh = readReg(REG_P1_YH);
    uint8_t yl = readReg(REG_P1_YL);

    x = ((xh & 0x0F) << 8) | xl;
    y = ((yh & 0x0F) << 8) | yl;
    return true;
}

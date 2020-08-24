#pragma once

#include <stdint.h>

class ILedMatrix {
  public:
    virtual ~ILedMatrix() {};
    virtual uint8_t getWidth() = 0;
    virtual uint8_t getHeight() = 0;
    virtual void clearDisplay() = 0;
    virtual void fillDisplay() = 0;
    virtual void enableDisplay(bool state) = 0;
    virtual void setBrightness(uint16_t brightnessPercent) = 0; // 0xFF = 100%, 0x00 = 0%
    virtual void updateDisplay() = 0;
    virtual void setPixel(uint8_t x, uint8_t y, bool state) = 0;
    //virtual void writeChararcter(uint8_t x, uint8_t y, Character& char) = 0;
};

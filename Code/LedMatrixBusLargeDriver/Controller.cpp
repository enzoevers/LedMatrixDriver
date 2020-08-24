#pragma once

#define ARDUINO_PLATFORM
#include "Controller.h"
#include "Platforms.h"

Controller::Controller(ILedMatrix& ledMatrix)
  : m_ledMatrix(ledMatrix)
{
  
};

void Controller::test()
{
  m_ledMatrix.enableDisplay(false);
  m_ledMatrix.clearDisplay();
  m_ledMatrix.updateDisplay();
  m_ledMatrix.enableDisplay(true);

  PlatformAbstraction::timingHandling.waitMilliseconds(1000);

  for (uint8_t y = 0; y < m_ledMatrix.getHeight(); y++) {
    for (uint8_t x = 0; x < m_ledMatrix.getWidth(); x++) {
      m_ledMatrix.setPixel(x, y, 1);

      m_ledMatrix.enableDisplay(false);
      m_ledMatrix.updateDisplay();
      m_ledMatrix.enableDisplay(true);
      PlatformAbstraction::timingHandling.waitMicroseconds(100);
    }
  }

  PlatformAbstraction::timingHandling.waitMilliseconds(1000);
  
  for(int32_t b = 0xFFFF; b >= 0; b -=10) {
    m_ledMatrix.setBrightness(0xFFFF & b);
    PlatformAbstraction::timingHandling.waitMilliseconds(2);
  }

  PlatformAbstraction::timingHandling.waitMilliseconds(1000);
}

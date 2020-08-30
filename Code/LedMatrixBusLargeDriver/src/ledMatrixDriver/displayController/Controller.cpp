#include <ledMatrixDriver/displayController/Controller.h>
#include <ledMatrixDriver/driverPlatforms/Platforms.h>

#include <iostream>

Controller::Controller(ILedMatrix& ledMatrix)
  : m_ledMatrix(ledMatrix)
{
  
};

void Controller::clearDisplay()
{
  m_ledMatrix.enableDisplay(false);
  m_ledMatrix.clearDisplay();
  m_ledMatrix.updateDisplay();
  m_ledMatrix.enableDisplay(true);
}

void Controller::fillDisplay()
{
  m_ledMatrix.enableDisplay(false);
  m_ledMatrix.fillDisplay();
  m_ledMatrix.updateDisplay();
  m_ledMatrix.enableDisplay(true);
}

void Controller::test()
{
  std::cout << "Clearing display\n";
  m_ledMatrix.enableDisplay(false);
  m_ledMatrix.clearDisplay();
  m_ledMatrix.updateDisplay();
  m_ledMatrix.enableDisplay(true);

  PlatformAbstraction::timingHandling.waitMilliseconds(1000);

  std::cout << "Filling display pixel-by-pixel\n";
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
  
  /*
  std::cout << "Fade out display\n";
  for(int32_t b = 0xFFFF; b >= 0; b--) {
    m_ledMatrix.setBrightness(0xFFFF & b);
    PlatformAbstraction::timingHandling.waitMicroseconds(100);
  }

  PlatformAbstraction::timingHandling.waitMilliseconds(1000);
  */
}
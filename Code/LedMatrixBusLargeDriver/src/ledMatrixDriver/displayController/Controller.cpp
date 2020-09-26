#include <ledMatrixDriver/displayController/Controller.h>
#include <ledMatrixDriver/driverPlatforms/Platforms.h>

#include <iostream>
#include <bitset>


Controller::Controller(ILedMatrix& ledMatrix, ICharacterProvider& characterProvider)
  : m_ledMatrix(ledMatrix),
    m_characterProvider(characterProvider)
{
  
};

void Controller::showText(std::string text)
{
  ContentData textMap;
  m_characterProvider.getText(text, textMap);

  std::cout << "textMap.height: " << int(textMap.height) << std::endl;
  std::cout << "textMap.width: " << int(textMap.width) << std::endl;

  for(int i = 0; i < 32; i++) {
    std::cout << std::bitset<32>(textMap.contentMask[i]) << "\n";
  }

  for (uint8_t y = 0; y < textMap.height; y++) {
    uint32_t curRow = textMap.contentMask[y];
    for (uint8_t x = 0; x < textMap.width; x++) {
      if(curRow & (0x1 << (31-x)) ) {
        std::cout << "Enable pixel on x: " << int(x) << " y: " << int(y) << std::endl;
        m_ledMatrix.setPixel(x, y, 1);
      } else {
        m_ledMatrix.setPixel(x, y, 0);
      }
    }
  }

  m_ledMatrix.enableDisplay(false);
  m_ledMatrix.updateDisplay();
  m_ledMatrix.enableDisplay(true);
}

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
  
  /*
  std::cout << "Fade out display\n";
  for(int32_t b = 0xFFFF; b >= 0; b--) {
    m_ledMatrix.setBrightness(0xFFFF & b);
    PlatformAbstraction::timingHandling.waitMicroseconds(100);
  }

  PlatformAbstraction::timingHandling.waitMilliseconds(1000);
  */
}
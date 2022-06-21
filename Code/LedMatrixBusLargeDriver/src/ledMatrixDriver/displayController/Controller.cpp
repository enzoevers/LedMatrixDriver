#include <ledMatrixDriver/displayController/Controller.h>
#include <ledMatrixDriver/driverPlatforms/Platforms.h>

#include <iostream>
#include <bitset>


Controller::Controller(ILedMatrix& ledMatrix, ICharacterProvider& characterProvider)
  : m_ledMatrix(ledMatrix),
    m_characterProvider(characterProvider)
{
  
};

void Controller::showText(std::string text, int16_t xOffset, int16_t yOffset, bool isTransparant, bool updateDisplayNow)
{
  ContentData* textMap = new ContentData();
  static bool needExtraPass = false;

  needExtraPass = m_characterProvider.getText(text, *textMap, needExtraPass);

  //std::cout << "textMap.height: " << int(textMap.height) << std::endl;
  //std::cout << "textMap.width: " << int(textMap.width) << std::endl;

  //for(int i = 0; i < textMap.maxRows; i++) {
  //  std::cout << std::bitset<64>(textMap.contentMask[i]) << "\n";
  //}

  for (uint8_t y = 0; y < textMap->height; y++) {
    uint64_t curRow = textMap->contentMask[y];
    for (uint8_t x = 0; x < textMap->width; x++) {

      if(x+xOffset < m_ledMatrix.getWidth() && x+xOffset >= 0 &&
          y+yOffset < m_ledMatrix.getHeight() && y+yOffset >= 0) {

        if(curRow & ((uint64_t)0x1 << ((textMap->maxRowWidth-1)-x)) ) {
          //std::cout << "Enable pixel on x: " << int(x) << " y: " << int(y) << std::endl;
            m_ledMatrix.setPixel(x+xOffset, y+yOffset, 1);
        } else {
          if(!isTransparant) {
            m_ledMatrix.setPixel(x+xOffset, y+yOffset, 0);
          }
        }

      }

    }
  }

  uint8_t textMap_width = textMap->width;

  delete textMap;
  textMap = nullptr;

  if(needExtraPass == true) {
    showText(text, xOffset + textMap_width, yOffset);
  }

  // Reset static variables
  needExtraPass = false;

  if(updateDisplayNow) {
    updateDisplay();
  }
}

void Controller::clearDisplay(bool updateDisplayNow)
{
  m_ledMatrix.clearDisplay();

  if(updateDisplayNow) {
    updateDisplay();
  }
}

void Controller::fillDisplay(bool updateDisplayNow)
{
  m_ledMatrix.fillDisplay();

  if(updateDisplayNow) {
    updateDisplay();
  }
}

void Controller::setBrightness(uint16_t brightnessPercent)
{
  m_ledMatrix.setBrightness(brightnessPercent);
}

void Controller::updateDisplay()
{
  setBrightness(0x0000);
  m_ledMatrix.enableDisplay(false);
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

uint16_t Controller::getDisplayWidth()
{
  return m_ledMatrix.getWidth();
}

uint16_t Controller::getDisplayHeight()
{
  return m_ledMatrix.getHeight();
}
#pragma once

#include <string>
#include <ledMatrixDriver/targetDisplays/ILedMatrix.h>
#include <ledMatrixDriver/displayContent/ICharacterProvider.h>
//#include "ICharacterProvider.h"

class Controller {
  public:
    Controller(ILedMatrix& ledMatrix, ICharacterProvider& characterProvider);


    // TODO: Add in 'invert' option to showText.
    // Meaning the active character leds are turned of and the rest on
    void showText(std::string text, int16_t xOffset = 0, int16_t yOffset = 0, bool isTransparant = false, bool updateDisplayNow = true);
    void clearDisplay(bool updateDisplayNow = true);
    void fillDisplay(bool updateDisplayNow = true);
    void updateDisplay();
    void test();

    uint16_t getDisplayWidth();
    uint16_t getDisplayHeight();

  private:
    ILedMatrix& m_ledMatrix;
    ICharacterProvider& m_characterProvider;
};

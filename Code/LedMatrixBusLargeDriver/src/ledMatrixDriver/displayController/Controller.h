#pragma once

#include <string>
#include <ledMatrixDriver/targetDisplays/ILedMatrix.h>
#include <ledMatrixDriver/displayContent/ICharacterProvider.h>
//#include "ICharacterProvider.h"

class Controller {
  public:
    Controller(ILedMatrix& ledMatrix, ICharacterProvider& characterProvider);

    void showText(std::string text);
    void clearDisplay();
    void fillDisplay();
    void test();

  private:
    ILedMatrix& m_ledMatrix;
    ICharacterProvider& m_characterProvider;
};

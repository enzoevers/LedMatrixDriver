#pragma once

#include <ledMatrixDriver/targetDisplays/ILedMatrix.h>
//#include "ICharacterProvider.h"

class Controller {
  public:
    Controller(ILedMatrix& ledMatrix);

    void clearDisplay();
    void fillDisplay();
    void test();

  private:
    ILedMatrix& m_ledMatrix;
};

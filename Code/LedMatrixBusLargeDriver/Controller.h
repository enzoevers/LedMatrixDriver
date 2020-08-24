#pragma once

#include "ILedMatrix.h"
//#include "ICharacterProvider.h"

class Controller {
  public:
    Controller(ILedMatrix& ledMatrix);

    void test();

  private:
    ILedMatrix& m_ledMatrix;
};

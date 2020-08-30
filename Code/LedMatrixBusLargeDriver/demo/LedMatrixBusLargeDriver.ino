const uint8_t clkDestSelectPin = 13;
const uint8_t clkLedDriverPin = 12;
const uint8_t dataOutPin = 11;
const uint8_t clkPin = 10;
const uint8_t latchLedDriverPin = 9;
const uint8_t outputEnableLedDriverPin = 6;

#include "LedMatrix_Hanover_OL037A.h"
#include "Controller.h"

ILedMatrix* display = nullptr;
Controller* controller = nullptr;

void setup() {
  Serial.begin(115200);
  
  display = new LedMatrix_Hanover_OL037A(clkDestSelectPin,
                                         clkLedDriverPin,
                                         dataOutPin,
                                         clkPin,
                                         latchLedDriverPin,
                                         outputEnableLedDriverPin);

  controller = new Controller(*display);
}

void loop() {
  controller->test();
}

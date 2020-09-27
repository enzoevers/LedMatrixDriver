#include <ledMatrixDriver/targetDisplays/LedMatrix_Hanover_OL037A.h>
#include <ledMatrixDriver/driverPlatforms/Platforms.h>

LedMatrix_Hanover_OL037A::LedMatrix_Hanover_OL037A(uint8_t clkDestSelectPin,
    uint8_t clkLedDriverPin,
    uint8_t dataOutPin,
    uint8_t clkPin,
    uint8_t latchLedDriverPin,
    uint8_t outputEnableLedDriverPin)
  :   m_clkDestSelectPin(clkDestSelectPin),
      m_clkLedDriverPin(clkLedDriverPin),
      m_dataOutPin(dataOutPin),
      m_clkPin(clkPin),
      m_latchLedDriverPin(latchLedDriverPin),
      m_outputEnableLedDriverPin(outputEnableLedDriverPin)

{
  PlatformAbstraction::gpioHandling.setDirection(m_clkDestSelectPin, PlatformAbstraction::GpioDirection::DIR_OUTPUT);
  PlatformAbstraction::gpioHandling.setDirection(m_clkLedDriverPin, PlatformAbstraction::GpioDirection::DIR_OUTPUT);
  PlatformAbstraction::gpioHandling.setDirection(m_dataOutPin, PlatformAbstraction::GpioDirection::DIR_OUTPUT);
  PlatformAbstraction::gpioHandling.setDirection(m_clkPin, PlatformAbstraction::GpioDirection::DIR_OUTPUT);
  PlatformAbstraction::gpioHandling.setDirection(m_latchLedDriverPin, PlatformAbstraction::GpioDirection::DIR_OUTPUT);
  PlatformAbstraction::gpioHandling.setDirection(m_outputEnableLedDriverPin, PlatformAbstraction::GpioDirection::DIR_OUTPUT);

  PlatformAbstraction::gpioHandling.writeDigital(m_clkDestSelectPin, 1);
  PlatformAbstraction::gpioHandling.writeDigital(m_clkLedDriverPin, 0);
  PlatformAbstraction::gpioHandling.writeDigital(m_dataOutPin, 0);
  PlatformAbstraction::gpioHandling.writeDigital(m_clkPin, 0);
  PlatformAbstraction::gpioHandling.writeDigital(m_latchLedDriverPin, 0);
  PlatformAbstraction::gpioHandling.writeDigital(m_outputEnableLedDriverPin, 1);
}

//===============
// ILedMatrix
//===============

void LedMatrix_Hanover_OL037A::clearDisplay()
{
  for (uint8_t s = 0; s < m_numVerticalSections; s++) {
    for (uint8_t p = 0; p < m_numPanels; p++) {
      for (uint8_t c = 0; c < m_numColumnsPerPanel; c++) {
        displayData[s][p][c] = 0x00;
      }
      displaySectionChanged[s][p] = true;
    }
  }
}

void LedMatrix_Hanover_OL037A::fillDisplay()
{
  for (uint8_t s = 0; s < m_numVerticalSections; s++) {
    for (uint8_t p = 0; p < m_numPanels; p++) {
      for (uint8_t c = 0; c < m_numColumnsPerPanel; c++) {
        displayData[s][p][c] = 0xFF;
      }
      displaySectionChanged[s][p] = true;
    }
  }
}

void LedMatrix_Hanover_OL037A::enableDisplay(bool state)
{
  PlatformAbstraction::gpioHandling.writeDigital(m_outputEnableLedDriverPin, !state); // Active-low

  // Active-low
  /*if(state) {
    PlatformAbstraction::gpioHandling.writePwm(m_outputEnableLedDriverPin, 0x0);
  } else {
    PlatformAbstraction::gpioHandling.writePwm(m_outputEnableLedDriverPin, 0xFFFF);
  }*/
}

void LedMatrix_Hanover_OL037A::setBrightness(uint16_t brightnessPercent) 
{
  //PlatformAbstraction::gpioHandling.writePwm(m_outputEnableLedDriverPin, 0xFFFF - brightnessPercent); // Generates an active-low PWM waveform
}

void LedMatrix_Hanover_OL037A::updateDisplay()
{
  for (uint8_t s = 0; s < m_numVerticalSections; s++) {
    for (uint8_t p = 0; p < m_numPanels; p++) {
      if (displaySectionChanged[s][p] == true) {
        writeSection(p, s);
        displaySectionChanged[s][p] = false;
      }
    }
  }
}

void LedMatrix_Hanover_OL037A::setPixel(uint8_t x, uint8_t y, bool state)
{
  // 0 <= y < 8 => section 0
  // 8 <= y < 16 => section 1
  // 16 <= y < 24 => section 2
  uint8_t section = y / m_numRowsPerSections;
  if (section == 1 || section == 2) {
    section ^= 0b11; // Reversed binary section select (1 -> 0b10 and 2 -> 0b01)
  }

  // 0 <= x < 40 => panel 0
  // 40 <= x < 80 => panel 1
  // 80 <= x < 120 => panel 2
  // 120 <= x < 160 => panel 3
  const uint8_t panel = x / m_numColumnsPerPanel;

  const uint8_t column = x - (panel * m_numColumnsPerPanel);

  if (state) {
    displayData[section][panel][column] |= (0b1 << (y % m_numRowsPerSections));
  }
  else {
    displayData[section][panel][column] &= ~(0b1 << (y % m_numRowsPerSections));
  }

  displaySectionChanged[section][panel] = true;
}

//===============
// Private
//===============

void LedMatrix_Hanover_OL037A::writeSection(uint8_t panel, uint8_t section)
{
  const unsigned int delayBetweenIoOperation_us = 8;
  if(m_lastUpdatedPanel != panel || m_lastUpdatedSection != section) {
    m_lastUpdatedPanel = panel;
    m_lastUpdatedSection = section;

    //==========
    // Select panel and section
    //==========

    // Select the clock for the panel & section select shift register
    PlatformAbstraction::gpioHandling.writeDigital(m_clkDestSelectPin, 1);
    PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);

    // panelSectionSelectData[7:6] = reversed binary section select (1 -> 0b10 and 2 -> 0b01)
    // panelSectionSelectData[5:0] = inverted one-hot panel select

    const uint8_t panelSectionSelectData = 0b0 | ((section & 0b11) << 6) | (0x3F & ~(0b1 << panel));

    for (int8_t i = m_numRowsPerSections-1; i >= 0; i--) {
      PlatformAbstraction::gpioHandling.writeDigital(m_clkPin, 0);
      PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
      PlatformAbstraction::gpioHandling.writeDigital(m_dataOutPin, 0b1 & (panelSectionSelectData >> i));
      PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
      PlatformAbstraction::gpioHandling.writeDigital(m_clkPin, 1); // Clock the data into the 74HC164 shift registers for the selection
      PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
    }
  }

  //==========
  // Write data
  //==========
  // Select the clock for the LED data shift register
  PlatformAbstraction::gpioHandling.writeDigital(m_clkDestSelectPin, 0);
  PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
  
  for (uint8_t c = 0; c < m_numColumnsPerPanel; c++) {
    PlatformAbstraction::gpioHandling.writeDigital(m_clkLedDriverPin, 0);
    PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
    PlatformAbstraction::gpioHandling.writeDigital(m_latchLedDriverPin, 0);
    PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);

    uint8_t columnData = displayData[section][panel][c];

    for (uint8_t i = 0; i < m_numRowsPerSections; i++) {
      PlatformAbstraction::gpioHandling.writeDigital(m_clkPin, 0);
      PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
      PlatformAbstraction::gpioHandling.writeDigital(m_dataOutPin, (columnData & (0b1 << i)));
      PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
      PlatformAbstraction::gpioHandling.writeDigital(m_clkPin, 1); // Clock the data into the 74HC164 shift registers for the LED data
      PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
    }
    PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
    PlatformAbstraction::gpioHandling.writeDigital(m_clkLedDriverPin, 1); // Clock the data into the MBI5167G LED drivers
    PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
    PlatformAbstraction::gpioHandling.writeDigital(m_latchLedDriverPin, 1); // Latch the data to the MBI5167G LED driver outputs
    PlatformAbstraction::timingHandling.waitMicroseconds(delayBetweenIoOperation_us);
  }
}

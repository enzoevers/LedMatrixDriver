#pragma once

#include "ILedMatrix.h"

class LedMatrix_Hanover_OL037A : public ILedMatrix {
    public:
        LedMatrix_Hanover_OL037A(uint8_t clkDestSelectPin,
                                    uint8_t clkLedDriverPin,
                                    uint8_t dataOutPin,
                                    uint8_t clkPin,
                                    uint8_t latchLedDriverPin,
                                    uint8_t outputEnableLedDriverPin);
        ~LedMatrix_Hanover_OL037A() =default;

        //===============
        // ILedMatrix
        //===============
        uint8_t getWidth() override {return m_numColums;};
        uint8_t getHeight() override {return m_numRows;};
        void clearDisplay() override;
        void fillDisplay() override;
        void enableDisplay(bool state) override;
        void setBrightness(uint16_t brightnessPercent) override;
        void updateDisplay() override;
        void setPixel(uint8_t x, uint8_t y, bool state) override;
    
    private:
        //===============
        // Display control pins
        //===============
        const uint8_t m_clkDestSelectPin = 0;
        const uint8_t m_clkLedDriverPin = 0;
        const uint8_t m_dataOutPin = 0;
        const uint8_t m_clkPin = 0;
        const uint8_t m_latchLedDriverPin = 0;
        const uint8_t m_outputEnableLedDriverPin = 0;

        //===============
        // Display data
        //===============
        static const uint8_t m_numPanels = 4;
        static const uint8_t m_numVerticalSections = 3;
        static const uint8_t m_numRowsPerSections = 8;
        static const uint8_t m_numColumnsPerPanel = 40;
        static const uint8_t m_numRows = 19;
        static const uint8_t m_numColums = m_numColumnsPerPanel * m_numPanels;

        // displayData[0][0][0] means:
        //      - Section 0 (top section)
        //      - Panel 0 (most left)
        //      - column 0 of panel 0 (most left columns of panel 0)
        //
        // Memory layout for 3d array: https://eli.thegreenplace.net/2015/memory-layout-of-multi-dimensional-arrays
        //
        // The data of a single index contains the data for one column in a section
        // with bit 0 corresponding to the top row of that section.
        //
        // Example:
        //      If the LED in row 10, column 54 should be on then displayData[1][1][14] should be 0b00000100
        //      Section 1 (second/middle section since: 7 < row 10 < 16), panel 1 (since 40 <= column 54 < 79) and column 14 (since 40 + 14 = 54) 
        uint8_t displayData[m_numVerticalSections][m_numPanels][m_numColumnsPerPanel] = {};

        // Keeps track of changes made to a certain section. This is used to
        // prevent updating all sections of only one or two section are actually changed.
        // This makes updating the display more efficient.
        // When updateDisplay() is called all sections are set to false again.
        bool displaySectionChanged[m_numVerticalSections][m_numPanels] = {};
        

        // This function handles writing data to a section of a given panel.
        void writeSection(uint8_t panel, uint8_t section);
};

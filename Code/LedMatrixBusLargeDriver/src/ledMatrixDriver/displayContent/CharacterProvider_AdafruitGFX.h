#pragma once

#include <LedMatrixDriver/displayContent/ICharacterProvider.h>

#include <Adafruit_GFX/gfxfonts.h>
// Fonts
#include <Adafruit_GFX/Fonts/FreeMono12pt7b.h>
//#include <Adafruit_GFX/Fonts/....h>
//#include <Adafruit_GFX/Fonts/....h>
//etc

class CharacterProvider_AdafruitGFX : ICharacterProvider {
    public:
        ~CharacterProvider_AdafruitGFX() =default;
        
        //===============
        // ICharacterProvider
        //===============
        bool setFont(GFXfont& font) override;
        bool getCharacter(uint16_t characterID, ContentData& contentStructToFill) override;

    private:
        GFXfont* m_activeFont = &FreeMono12pt7b;
        uint8_t m_totalGlyphHeigt = 0;
        int8_t m_glyphBaseLineInTotalHeight = 0;
};
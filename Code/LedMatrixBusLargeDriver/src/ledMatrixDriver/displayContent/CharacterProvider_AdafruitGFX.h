#pragma once

#include <ledMatrixDriver/displayContent/ICharacterProvider.h>

// Fonts
#include <Adafruit_GFX/gfxfont.h>
#include <Adafruit_GFX/Fonts/FreeMono12pt7b.h>
//#include <Adafruit_GFX/Fonts/....h>
//#include <Adafruit_GFX/Fonts/....h>
//etc

class CharacterProvider_AdafruitGFX : public ICharacterProvider {
    public:
        CharacterProvider_AdafruitGFX();
        ~CharacterProvider_AdafruitGFX() =default;
        
        //===============
        // ICharacterProvider
        //===============
        bool setFont(GFXfont& font) override;
        bool getText(std::string text, ContentData& contentStructToFill) override;

    private:
        GFXfont* m_activeFont =  nullptr;
        uint8_t m_totalGlyphHeigt = 0;
        int8_t m_glyphBaseLineInTotalHeight = 0;
};
#pragma once

#include <ledMatrixDriver/displayContent/ICharacterProvider.h>
#include <Adafruit_GFX/gfxfont.h>


class CharacterProvider_AdafruitGFX : public ICharacterProvider {
    public:
        CharacterProvider_AdafruitGFX();
        ~CharacterProvider_AdafruitGFX() =default;
        
        //===============
        // ICharacterProvider
        //===============
        bool setFont(GFXfont& font) override;
        bool getText(std::string text, ContentData& contentStructToFill, bool contentOverflow = false) override;

    private:
        GFXfont* m_activeFont =  nullptr;
        uint8_t m_totalGlyphHeigt = 0;
        int8_t m_glyphBaseFromTop = 0;

        int16_t m_cursorX = 0;
        std::string m_currentText;
        uint16_t m_currentTextIndex = 0;
};
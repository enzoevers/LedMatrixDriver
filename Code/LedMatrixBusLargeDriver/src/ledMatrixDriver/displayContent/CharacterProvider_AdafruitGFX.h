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
        bool getText(std::string text, ContentData& contentStructToFill) override;

    private:
        GFXfont* m_activeFont =  nullptr;
        uint8_t m_totalGlyphHeigt = 0;
        int8_t m_glyphBaseFromTop = 0;
};
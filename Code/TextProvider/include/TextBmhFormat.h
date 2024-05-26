#pragma once

#include "Fonts/BmhFont.h"  // Generated after cmake configuration
#include "ITextProvider.h"
#include "PixelArea.h"

class TextBmhFormat : public ITextProvider {
   public:
    ~TextBmhFormat() override = default;

    //=========================
    // ITextProvider
    //=========================
    auto GetRequiredSizeCharacter(char character) -> Vec2D override;
    auto GetRequiredSizeString(const std::string& text, uint8_t spacing) -> Vec2D override;
    auto SetString(const std::string& text, uint8_t spacing, Vec2D offset, PixelArea<Monochrome>& pixelArea)
        -> bool override;

    //=========================
    // TextBmhFormat
    //=========================
    /*!
        @return False if there are any invalid/nullptr values in the font struct. True otherwise.
    */
    auto SetFont(const BmhFont* font) -> bool;
    auto GetFont() -> const BmhFont*;

   private:
    /*!
        @return std::numeric_limits<uint8_t>::max(); if the character is not available.
                Otherwise, the index of the character in the available_chars array.
    */
    auto GetCharacterIndex(const char character) -> uint8_t;

    BmhFont* m_font = nullptr;
};
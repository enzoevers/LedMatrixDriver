#pragma once

#include <stdint.h>

#include <string>

#include "Monochrome.h"
#include "PixelArea.h"
#include "VecXD.h"

class ITextProvider {
   public:
    virtual ~ITextProvider() = default;

    /*!
        @return The dimensions required to show the character
    */
    virtual auto GetRequiredSizeCharacter(char character) const -> const Vec2D = 0;

    /*!
        @param string A string to show.
        @param spacing The amount of pixels between characters in the string.
        @return The dimensions required to show the string.
    */
    virtual auto GetRequiredSizeString(const std::string& text, uint8_t spacing) const -> const Vec2D = 0;

    /*!
        @param string A string to show.
        @param spacing The amount of pixels between characters in the string. The pixels in this space are not touched.
        @param offset The offset from the top-left of the pixelArea.
        @return Success.
    */
    virtual auto SetString(const std::string& text, uint8_t spacing, Vec2D offset, PixelArea<Monochrome>& pixelArea)
        -> bool = 0;

    // Note: when new colors are required, copy paste the SetString(...) function interface
};
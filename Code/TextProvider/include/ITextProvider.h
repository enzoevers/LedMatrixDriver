#pragma once

#include <stdint.h>

#include "Monochrome.h"
#include "PixelArea.h"
#include "VecXD.h"

class ITextProvider {
   public:
    virtual ~ITextProvider() = default;

    /*!
        @return The dimensions required to show the character
    */
    virtual auto GetRequiredSizeCharacter(char character) -> Vec2D;

    /*!
        @param string A null-terminated string
        @param spacing The amount of pixels between characters in the string
        @return The dimensions required to show the string
    */
    virtual auto GetRequiredSizeString(char* string, uint8_t spacing) -> Vec2D;

    /*!
        @param string A null-terminated string
        @param spacing The amount of pixels between characters in the string
        @param offset The offset from the top-left of the
        @return success
    */
    virtual auto SetString(char* string, uint8_t spacing, Vec2D offset, PixelArea<Monochrome>& pixelArea) -> bool;

    // Note: when new colors are required, copy paste the SetString(...) function interface
};
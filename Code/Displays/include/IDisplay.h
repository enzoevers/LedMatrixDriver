#pragma once

#include <stdint.h>

#include "PixelArea.h"
#include "XYPair.h"

template <typename Color>
class IDisplay {
   public:
    virtual ~IDisplay() = default;

    virtual auto getResolution() -> XYPair = 0;
    virtual auto getCurrentDisplay() -> const PixelArea<Color>& = 0;
    virtual auto getCurrentPixelArea(XYPair topLeftoordinate, uint32_t width, uint32_t height)
        -> const PixelArea<Color>& = 0;
    virtual auto setPixel(XYPair coordinate, Color color) -> bool = 0;
    virtual auto setArea(XYPair topLeftoordinate, const PixelArea<Color>& pixelArea) -> bool = 0;
    virtual auto updateDisplay() -> bool = 0;
};
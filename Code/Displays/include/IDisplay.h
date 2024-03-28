#pragma once

#include <stdint.h>

#include "PixelArea.h"
#include "XYPair.h"

template <typename Color>
class IDisplay {
   public:
    virtual ~IDisplay() = default;

    virtual auto GetResolution() -> XYPair = 0;
    virtual auto GetCurrentDisplay() -> const PixelArea<Color>& = 0;
    virtual auto GetCurrentPixelArea(XYPair topLeftoordinate, uint32_t width, uint32_t height)
        -> const PixelArea<Color>& = 0;
    virtual auto SetPixel(XYPair coordinate, Color color) -> bool = 0;
    virtual auto SetArea(XYPair topLeftoordinate, const PixelArea<Color>& pixelArea) -> bool = 0;
    virtual auto UpdateDisplay() -> bool = 0;
};
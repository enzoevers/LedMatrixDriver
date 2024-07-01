#pragma once

#include <stdint.h>

#include "PixelArea.h"
#include "VecXD.h"

template <typename Color>
class IDisplay {
   public:
    virtual ~IDisplay() = default;

    virtual auto GetResolution() const -> Vec2D = 0;
    virtual auto GetCurrentDisplay(PixelArea<Color>& pixelArea) const -> void = 0;
    virtual auto GetCurrentPixelArea(const Vec2D topLeftoordinate, PixelArea<Color>& pixelArea) const -> bool = 0;
    virtual auto SetPixel(const Vec2D coordinate, const Color color) -> bool = 0;
    virtual auto SetArea(const Vec2D topLeftoordinate, const PixelArea<Color>& pixelArea) -> bool = 0;
    virtual auto UpdateDisplay() -> bool = 0;
    // TODO: Make an `enableDisplay()` function
};
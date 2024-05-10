#pragma once

#include <stdint.h>

#include <limits>

#include "VecXD.h"

template <typename Color>
struct PixelArea {
    Vec2D size = {0, 0};
    Color* pixels = nullptr;

    auto at(Vec2D coordinateInArea) -> Color {
        if (coordinateInArea.x >= size.x || coordinateInArea.y >= size.y) {
            return std::numeric_limits<Color>::min();
        }

        std::size_t index = coordinateInArea.y * size.x + coordinateInArea.x;
        return pixels[index];
    }
};
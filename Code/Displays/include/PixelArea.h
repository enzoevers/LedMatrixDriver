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

    auto Set(Vec2D coordinateInArea, Color newColor) -> bool {
        if (coordinateInArea.x >= size.x || coordinateInArea.y >= size.y) {
            return false;
        }

        std::size_t index = coordinateInArea.y * size.x + coordinateInArea.x;
        pixels[index] = newColor;

        return true;
    }

    auto HasSameData(const PixelArea<Color>& other) const -> bool {
        if (size != other.size) {
            return false;
        }

        if (pixels == other.pixels) {
            return true;
        }

        if (pixels == nullptr || other.pixels == nullptr) {
            return false;
        }

        for (auto i = 0; i < size.x * size.y; ++i) {
            if (pixels[i] != other.pixels[i]) {
                return false;
            }
        }

        return true;
    }

    auto operator==(const PixelArea<Color>& other) const -> bool {
        return size == other.size && pixels == other.pixels;
    }
};
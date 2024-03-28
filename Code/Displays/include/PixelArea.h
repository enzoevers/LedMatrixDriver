#pragma once

#include <stdint.h>

#include <span>

template <typename Color>
struct PixelArea {
    uint32_t width;
    uint32_t height;
    Color* pixels;
};
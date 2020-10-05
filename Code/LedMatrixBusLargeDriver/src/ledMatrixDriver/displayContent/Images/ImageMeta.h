#pragma once

#include <stdint.h>

// https://onlinepngtools.com/pixelate-png

typedef struct {
    const uint16_t width = 0;
    const uint16_t height = 0;
    const uint8_t* imageData = nullptr;
} ImageMeta;
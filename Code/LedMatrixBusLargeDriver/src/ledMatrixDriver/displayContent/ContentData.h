#pragma once

#include <stdint.h>

typedef struct ContentData {
    uint8_t width = 0;
    uint8_t height = 0;

    // The contentMask is a fixed size cache for characters read from an external source.
    // The actual used size is given in the 'width' and 'height' variables.
    //
    // Each index represents a single row. Each bit in a single uint32_t value represents a single led.
    uint32_t contentMask[32] = {};

    void clearData() {
        width = 0;
        height = 0;
        contentMask = {};
    }
};

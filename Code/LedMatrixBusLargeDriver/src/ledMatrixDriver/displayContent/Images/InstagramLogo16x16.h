#pragma once

#include <ledMatrixDriver/displayContent/Images/ImageMeta.h>

// TODO: Add the centre lens and flash
const uint8_t InstagramLogo16x16Bitmap[((16/8) * 16)] = {
    0x3F, 0xFC,
    0x3F, 0xFC,
    0x70, 0x0E,
    0x70, 0x0E,
    0xF0, 0x0F,
    0xF0, 0x0F,
    0xF0, 0x0F, 
    0xF0, 0x0F,
    0xF0, 0x0F,
    0xF0, 0x0F,
    0xF0, 0x0F,
    0xF0, 0x0F,
    0x70, 0x0E,
    0x70, 0x0E,
    0x3F, 0xFC,
    0x3F, 0xFC
};

const ImageMeta InstagramLogo16x16 = {
    16,
    16,
    InstagramLogo16x16Bitmap
};
#pragma once

#include "Fonts/BmhFont.h"

// Header File for characters
// Generated with TTF2BMH
namespace Bitstream_Vera_Sans_Mono_12 {

const uint8_t bitmap_32[] = {0b0, 0b0};              // ' '
const uint8_t bitmap_33[] = {0b11100000, 0b110111};  // '!'
const uint8_t bitmap_49[] = {0b100000, 0b100000, 0b11100000, 0b0,      0b0,
                             0b100000, 0b100000, 0b111111,   0b100000, 0b100000};  // '1'
const uint8_t bitmap_50[] = {0b1000000, 0b100000, 0b100000, 0b100000, 0b100000, 0b1100000, 0b11000000,
                             0b100000,  0b110000, 0b101000, 0b101100, 0b100100, 0b100010,  0b100001};  // '2'
const uint8_t bitmap_51[] = {0b1000000, 0b100000, 0b100000, 0b100000, 0b100000, 0b100000, 0b11000000,
                             0b10000,   0b100000, 0b100010, 0b100010, 0b100010, 0b110101, 0b11101};  // '3'
const uint8_t bitmap_52[] = {0b0,    0b0,    0b10000000, 0b11000000, 0b100000, 0b11100000, 0b0,
                             0b1100, 0b1010, 0b1001,     0b1000,     0b1000,   0b111111,   0b1000};  // '4'
const uint8_t bitmap_53[] = {0b11100000, 0b100000, 0b100000, 0b100000, 0b100000, 0b100000, 0b0,
                             0b10001,    0b100001, 0b100001, 0b100001, 0b100001, 0b10010,  0b11110};              // '5'
const uint8_t bitmap_63[] = {0b1000000, 0b100000, 0b100000, 0b100000, 0b11000000, 0b0, 0b0, 0b110110, 0b1, 0b0};  // '?'
const uint8_t bitmap_65[] = {0b0,      0b0,     0b11000000, 0b100000, 0b11000000, 0b0,     0b0,
                             0b110000, 0b11110, 0b1001,     0b1000,   0b1001,     0b11110, 0b110000};  // 'A'
const uint8_t bitmap_66[] = {0b11100000, 0b100000, 0b100000, 0b100000, 0b100000, 0b100000, 0b11000000,
                             0b111111,   0b100010, 0b100010, 0b100010, 0b100010, 0b100010, 0b11101};  // 'B'
const uint8_t bitmap_68[] = {0b11100000, 0b100000, 0b100000, 0b100000, 0b100000, 0b1000000, 0b10000000,
                             0b111111,   0b100000, 0b100000, 0b100000, 0b100000, 0b10000,   0b1111};  // 'D'
const uint8_t bitmap_97[] = {0b0,     0b0,      0b10000000, 0b10000000, 0b10000000, 0b0,
                             0b11000, 0b100101, 0b100100,   0b100100,   0b10100,    0b111111};  // 'a'
const uint8_t bitmap_98[] = {0b11110000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b0,
                             0b111111,   0b110001,   0b100000,   0b100000,   0b110001,   0b11111};  // 'b'
const uint8_t bitmap_100[] = {0b0,     0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b11110000,
                              0b11111, 0b110001,   0b100000,   0b100000,   0b110001,   0b111111};  // 'd'
const uint8_t char_width[] = {1, 1, 5, 7, 7, 7, 7, 5, 7, 7, 7, 6, 6, 6};
const uint8_t* char_addr[] = {&bitmap_32[0], &bitmap_33[0], &bitmap_49[0], &bitmap_50[0], &bitmap_51[0],
                              &bitmap_52[0], &bitmap_53[0], &bitmap_63[0], &bitmap_65[0], &bitmap_66[0],
                              &bitmap_68[0], &bitmap_97[0], &bitmap_98[0], &bitmap_100[0]};
const char available_chars[] = {32, 33, 49, 50, 51, 52, 53, 63, 65, 66, 68, 97, 98, 100};
const uint8_t char_count = 14;
const uint8_t font_size = 12;
const uint8_t top_padding = 4;

const struct BmhFont font_data = {.char_width = char_width,
                                  .char_addr = char_addr,
                                  .available_chars = available_chars,
                                  .char_count = char_count,
                                  .font_size = font_size,
                                  .top_padding = top_padding};

};  // namespace Bitstream_Vera_Sans_Mono_12

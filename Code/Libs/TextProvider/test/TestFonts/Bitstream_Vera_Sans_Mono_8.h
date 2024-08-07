#pragma once

#include "Fonts/BmhFont.h"

// Header File for characters
// Generated with TTF2BMH
namespace Bitstream_Vera_Sans_Mono_8 {

const uint8_t bitmap_32[] = {0b0};                                             // ' '
const uint8_t bitmap_33[] = {0b10111100};                                      // '!'
const uint8_t bitmap_49[] = {0b10000100, 0b11111100, 0b10000000};              // '1'
const uint8_t bitmap_50[] = {0b10000100, 0b11000100, 0b10100100, 0b10011000};  // '2'
const uint8_t bitmap_51[] = {0b10000100, 0b10100100, 0b10100100, 0b11011000};  // '3'
const uint8_t bitmap_52[] = {0b1100000, 0b1011000, 0b11111100, 0b1000000};     // '4'
const uint8_t bitmap_53[] = {0b10011100, 0b10010100, 0b10010100, 0b1100100};   // '5'
const uint8_t bitmap_63[] = {0b100, 0b10111100, 0b1100};                       // '?'
const uint8_t bitmap_65[] = {0b11000000, 0b1111100, 0b1111100, 0b11000000};    // 'A'
const uint8_t bitmap_66[] = {0b11111100, 0b10100100, 0b10100100, 0b11011000};  // 'B'
const uint8_t bitmap_68[] = {0b11111100, 0b10000100, 0b10000100, 0b1111000};   // 'D'
const uint8_t bitmap_97[] = {0b11110000, 0b10110000, 0b10110000, 0b11110000};  // 'a'
const uint8_t bitmap_98[] = {0b11111110, 0b10010000, 0b10010000, 0b1100000};   // 'b'
const uint8_t bitmap_100[] = {0b1100000, 0b10010000, 0b10010000, 0b11111110};  // 'd'
const uint8_t char_width[] = {1, 1, 3, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4};
const uint8_t* char_addr[] = {&bitmap_32[0], &bitmap_33[0], &bitmap_49[0], &bitmap_50[0], &bitmap_51[0],
                              &bitmap_52[0], &bitmap_53[0], &bitmap_63[0], &bitmap_65[0], &bitmap_66[0],
                              &bitmap_68[0], &bitmap_97[0], &bitmap_98[0], &bitmap_100[0]};
const char available_chars[] = {32, 33, 49, 50, 51, 52, 53, 63, 65, 66, 68, 97, 98, 100};
const uint8_t char_count = 14;
const uint8_t font_size = 8;
const uint8_t top_padding = 0;

const struct BmhFont font_data = {.char_width = char_width,
                                  .char_addr = char_addr,
                                  .available_chars = available_chars,
                                  .char_count = char_count,
                                  .font_size = font_size,
                                  .top_padding = top_padding};

};  // namespace Bitstream_Vera_Sans_Mono_8

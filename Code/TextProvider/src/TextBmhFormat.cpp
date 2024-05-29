#include "TextBmhFormat.h"

#include <algorithm>

static constexpr auto NotFoundIndex = std::numeric_limits<uint8_t>::max();

//=========================
// ITextProvider
//=========================

auto TextBmhFormat::GetRequiredSizeCharacter(char character) const -> const Vec2D {
    if (m_font == nullptr) {
        return {0, 0};
    }

    const auto index = GetCharacterIndex(character);
    if (index == NotFoundIndex) {
        return {0, 0};
    }

    return {m_font->char_width[index], m_font->font_size};
}
auto TextBmhFormat::GetRequiredSizeString(const std::string& text, uint8_t spacing) const -> const Vec2D {
    if (m_font == nullptr) {
        return {0, 0};
    }

    uint16_t width = 0;
    for (const auto character : text) {
        if (GetCharacterIndex(character) == NotFoundIndex) {
            return {0, 0};
        }

        width += GetRequiredSizeCharacter(character).x + spacing;
    }

    // Remove the last spacing
    width -= spacing;

    return {width, m_font->font_size};
}

auto TextBmhFormat::SetString(const std::string& text, uint8_t spacing, Vec2D offset, PixelArea<Monochrome>& pixelArea)
    -> bool {
    if (m_font == nullptr) {
        return false;
    }

    auto xStart = offset.x;

    for (const auto character : text) {
        const auto charIndex = GetCharacterIndex(character);
        if (charIndex == NotFoundIndex) {
            return false;
        }

        const auto charAddr = m_font->char_addr[charIndex];
        const auto charSize = GetRequiredSizeCharacter(character);
        const auto padding = m_font->top_padding;

        // Note that (charSize.y + padding) is always divisible by 8
        const auto heightBytes = (charSize.y + padding) / 8;
        for (auto yByte = 0; yByte < heightBytes; ++yByte) {
            const auto appliedPadding = yByte == 0 ? padding : 0;
            // Go over all bits in the byte
            for (auto y = 0; y < (8 - appliedPadding); ++y) {
                const auto yInPixelArea = offset.y + (yByte * 8) - (padding - appliedPadding) + y;
                if (yInPixelArea < 0) {
                    continue;
                }
                if (yInPixelArea >= pixelArea.size.y) {
                    break;
                }
                for (auto x = 0; x < charSize.x; ++x) {
                    const auto xInPixelArea = xStart + x;
                    if (xInPixelArea < 0) {
                        continue;
                    }
                    if (xInPixelArea >= pixelArea.size.x) {
                        break;
                    }
                    const auto charByte = charAddr[yByte * charSize.x + x];
                    const auto pixel = (charByte >> (y + appliedPadding)) & 0x1;

                    const auto pixelAreaCoordinate = Vec2D{xInPixelArea, yInPixelArea};
                    pixelArea.Set(pixelAreaCoordinate, pixel);
                }
            }
        }

        xStart += charSize.x + spacing;
    }

    return true;
}

//=========================
// TextBmhformat
//=========================

auto TextBmhFormat::SetFont(const BmhFont* font) -> bool {
    if (font == nullptr || font->char_width == nullptr || font->char_addr == nullptr ||
        font->available_chars == nullptr || font->char_count == 0 || font->top_padding > font->font_size) {
        return false;
    }

    const auto start = font->char_addr;
    const auto end = start + font->char_count;
    const auto foundIterator = std::find(start, end, nullptr);
    if (foundIterator != end) {
        return false;
    }

    m_font = const_cast<BmhFont*>(font);

    return true;
}

auto TextBmhFormat::GetFont() const -> const BmhFont* { return m_font; }

//=========================
// Private
//=========================

auto TextBmhFormat::GetCharacterIndex(const char character) const -> const uint8_t {
    if (m_font == nullptr) {
        return NotFoundIndex;
    }

    uint8_t index = 0;
    while (m_font->available_chars[index] != character) {
        if (++index >= m_font->char_count) {
            return NotFoundIndex;
        }
    }

    return index;
}
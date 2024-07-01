#pragma once

#include <gmock/gmock.h>

#include "ITextProvider.h"

class MockITextProvider : public ITextProvider {
   public:
    ~MockITextProvider() override = default;

    MOCK_METHOD(const Vec2D, GetRequiredSizeCharacter, (char character), (const, override));
    MOCK_METHOD(const Vec2D, GetRequiredSizeString, (const std::string& text, uint8_t spacing), (const, override));
    MOCK_METHOD(bool, SetString,
                (const std::string& text, uint8_t spacing, Vec2D offset, PixelArea<Monochrome>& pixelArea), (override));
};
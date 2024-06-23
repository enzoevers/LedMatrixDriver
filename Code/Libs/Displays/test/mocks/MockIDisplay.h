#pragma once

#include <gmock/gmock.h>

#include "IDisplay.h"
#include "Monochrome.h"

class MockIDisplayMonochrome : public IDisplay<Monochrome> {
   public:
    ~MockIDisplayMonochrome() override = default;

    MOCK_METHOD(Vec2D, GetResolution, (), (const, override));
    MOCK_METHOD(void, GetCurrentDisplay, (PixelArea<Monochrome> & pixelArea), (const, override));
    MOCK_METHOD(bool, GetCurrentPixelArea, (const Vec2D topLeftoordinate, PixelArea<Monochrome>& pixelArea),
                (const, override));
    MOCK_METHOD(bool, SetPixel, (const Vec2D coordinate, const Monochrome color), (override));
    MOCK_METHOD(bool, SetArea, (const Vec2D topLeftoordinate, const PixelArea<Monochrome>& pixelArea), (override));
    MOCK_METHOD(bool, UpdateDisplay, (), (override));
};
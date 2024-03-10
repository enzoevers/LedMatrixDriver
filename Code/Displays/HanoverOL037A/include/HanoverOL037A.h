#pragma once

#include "HanoverOL037A_GPIOInterface.h"
#include "IDisplay.h"
#include "Monochrome.h"

class HanoverOL037A : public IDisplay<Monochrome> {
   public:
    ~HanoverOL037A() override = default;

    auto SetGPIOInterface(HanoverOL037A_GPIOInterface& gpioInterface) -> void;

    //=========================
    // IDisplay<Monochrome>
    //=========================
    auto getResolution() -> XYPair override;
    auto getCurrentDisplay() -> const PixelArea<Monochrome>& override;
    auto getCurrentPixelArea(XYPair topLeftoordinate, uint32_t width, uint32_t height)
        -> const PixelArea<Monochrome>& override;
    auto setPixel(XYPair coordinate, Monochrome color) -> bool override;
    auto setArea(XYPair topLeftoordinate, const PixelArea<Monochrome>& pixelArea) -> bool override;
    auto updateDisplay() -> bool override;

   private:
    HanoverOL037A_GPIOInterface* gpioInterface;
};
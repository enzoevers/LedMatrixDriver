#pragma once

#include "HanoverOL037A_GPIOInterface.h"
#include "IDelay.h"
#include "IDisplay.h"
#include "Monochrome.h"

class HanoverOL037A : public IDisplay<Monochrome> {
   public:
    ~HanoverOL037A() override = default;

    auto SetGPIOInterface(HanoverOL037A_GPIOInterface& gpioInterface) -> void;
    auto SetDelayManager(IDelay& delauManager) -> void;

    //=========================
    // IDisplay<Monochrome>
    //=========================
    auto GetResolution() -> XYPair override;
    auto GetCurrentDisplay() -> const PixelArea<Monochrome>& override;
    auto GetCurrentPixelArea(XYPair topLeftoordinate, uint32_t width, uint32_t height)
        -> const PixelArea<Monochrome>& override;
    auto SetPixel(XYPair coordinate, Monochrome color) -> bool override;
    auto SetArea(XYPair topLeftoordinate, const PixelArea<Monochrome>& pixelArea) -> bool override;
    auto UpdateDisplay() -> bool override;

   private:
    HanoverOL037A_GPIOInterface* m_pGpioInterface;
    IDelay* m_pDelayManager;
};
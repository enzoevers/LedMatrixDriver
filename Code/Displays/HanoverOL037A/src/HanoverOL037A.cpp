#include "HanoverOL037A.h"

auto HanoverOL037A::SetGPIOInterface(HanoverOL037A_GPIOInterface& gpioInterface) -> void {}
auto HanoverOL037A::SetDelayManager(IDelay& delauManager) -> void {}

//=========================
// IDisplay<Monochrome>
//=========================
auto HanoverOL037A::GetResolution() -> XYPair {}
auto HanoverOL037A::GetCurrentDisplay() -> const PixelArea<Monochrome>& {}
auto HanoverOL037A::GetCurrentPixelArea(XYPair topLeftoordinate, uint32_t width, uint32_t height)
    -> const PixelArea<Monochrome>& {}
auto HanoverOL037A::SetPixel(XYPair coordinate, Monochrome color) -> bool {}
auto HanoverOL037A::SetArea(XYPair topLeftoordinate, const PixelArea<Monochrome>& pixelArea) -> bool {}
auto HanoverOL037A::UpdateDisplay() -> bool {}
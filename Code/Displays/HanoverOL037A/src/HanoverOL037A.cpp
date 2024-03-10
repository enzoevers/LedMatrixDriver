#include "HanoverOL037A.h"

auto HanoverOL037A::SetGPIOInterface(HanoverOL037A_GPIOInterface& gpioInterface) -> void {}

//=========================
// IDisplay<Monochrome>
//=========================
auto HanoverOL037A::getResolution() -> XYPair {}
auto HanoverOL037A::getCurrentDisplay() -> const PixelArea<Monochrome>& {}
auto HanoverOL037A::getCurrentPixelArea(XYPair topLeftoordinate, uint32_t width, uint32_t height)
    -> const PixelArea<Monochrome>& {}
auto HanoverOL037A::setPixel(XYPair coordinate, Monochrome color) -> bool {}
auto HanoverOL037A::setArea(XYPair topLeftoordinate, const PixelArea<Monochrome>& pixelArea) -> bool {}
auto HanoverOL037A::updateDisplay() -> bool {}
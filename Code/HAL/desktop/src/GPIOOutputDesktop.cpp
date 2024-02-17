#include "GPIOOutputDesktop.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputDesktop::SetState(bool on) -> void {}

auto GPIOOutputDesktop::GetState() -> bool {}

//---------------
// IGPIOOutputDesktop
//---------------
auto GPIOOutputDesktop::SetOutputRegister(uint32_t* outputRegister) -> void {}
auto GPIOOutputDesktop::GetOutputRegister() const -> uint32_t* {}

auto GPIOOutputDesktop::SetPinMask(uint32_t pinMask) -> void {}
auto GPIOOutputDesktop::GetPinMask() const -> uint32_t {}

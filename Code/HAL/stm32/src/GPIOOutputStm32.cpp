#include "GPIOOutputStm32.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputStm32::SetState(bool on) -> void {}

auto GPIOOutputStm32::GetState() -> bool {}

//---------------
// IGPIOOutputStm32
//---------------
auto GPIOOutputStm32::SetOutputRegister(uint32_t* outputRegister) -> void {}
auto GPIOOutputStm32::GetOutputRegister() const -> uint32_t* {}

auto GPIOOutputStm32::SetPinMask(uint32_t pinMask) -> void {}
auto GPIOOutputStm32::GetPinMask() const -> uint32_t {}

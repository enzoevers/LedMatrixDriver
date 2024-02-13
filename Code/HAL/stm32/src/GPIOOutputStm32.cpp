#include "GPIOOutputStm32.h"

GPIOOutputStm32::GPIOOutputStm32(uint32_t* outputRegister, uint32_t pinMask)
    : m_outputRegister(outputRegister), m_pinMask(pinMask) {}

auto GPIOOutputStm32::SetState(bool on) -> void {}

auto GPIOOutputStm32::GetState() -> bool {}

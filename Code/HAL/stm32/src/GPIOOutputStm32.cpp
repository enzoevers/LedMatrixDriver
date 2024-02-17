#include "GPIOOutputStm32.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputStm32::SetState(bool on) -> void {
    if (on) {
        *m_outputRegister |= m_pinMask;
    } else {
        *m_outputRegister &= ~m_pinMask;
    }
}

auto GPIOOutputStm32::GetState() -> bool { return *m_outputRegister & m_pinMask; }

//---------------
// IGPIOOutputStm32
//---------------
auto GPIOOutputStm32::SetOutputRegister(uint32_t* outputRegister) -> void { m_outputRegister = outputRegister; }
auto GPIOOutputStm32::GetOutputRegister() const -> const uint32_t* { return m_outputRegister; }

auto GPIOOutputStm32::SetPinMask(uint32_t pinMask) -> void { m_pinMask = pinMask; }
auto GPIOOutputStm32::GetPinMask() const -> uint32_t { return m_pinMask; }

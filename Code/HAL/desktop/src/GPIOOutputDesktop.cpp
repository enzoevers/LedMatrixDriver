#include "GPIOOutputDesktop.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputDesktop::SetState(bool on) -> void {
    if (on) {
        *m_outputRegister |= m_pinMask;
    } else {
        *m_outputRegister &= ~m_pinMask;
    }
}

auto GPIOOutputDesktop::GetState() -> bool { return *m_outputRegister & m_pinMask; }

//---------------
// IGPIOOutputDesktop
//---------------
auto GPIOOutputDesktop::SetOutputRegister(uint32_t* outputRegister) -> void { m_outputRegister = outputRegister; }
auto GPIOOutputDesktop::GetOutputRegister() const -> const uint32_t* { return m_outputRegister; }

auto GPIOOutputDesktop::SetPinMask(uint32_t pinMask) -> void { m_pinMask = pinMask; }
auto GPIOOutputDesktop::GetPinMask() const -> uint32_t { return m_pinMask; }

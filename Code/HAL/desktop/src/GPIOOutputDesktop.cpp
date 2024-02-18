#include <stdexcept>

#include "GPIOOutputDesktop.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputDesktop::SetState(bool on) -> bool {
    if (m_outputRegister == nullptr) {
        return false;
    }

    if (on) {
        *m_outputRegister |= m_pinMask;
    } else {
        *m_outputRegister &= ~m_pinMask;
    }

    return true;
}

auto GPIOOutputDesktop::GetState() -> bool {
    if (m_outputRegister == nullptr) {
        return false;
    }

    return *m_outputRegister & m_pinMask;
}

//---------------
// IGPIOOutputDesktop
//---------------
auto GPIOOutputDesktop::SetOutputRegister(uint32_t* outputRegister) -> void {
    if (outputRegister == nullptr) {
        throw std::invalid_argument("No nullptr allowed");
    }
    m_outputRegister = outputRegister;
}
auto GPIOOutputDesktop::GetOutputRegister() const -> const uint32_t* { return m_outputRegister; }

auto GPIOOutputDesktop::SetPinMask(uint32_t pinMask) -> void {
    if ((pinMask == 0) || (((pinMask & (pinMask - 1)) != 0))) {
        throw std::invalid_argument("Only 1 set bit allowed");
    }
    m_pinMask = pinMask;
}
auto GPIOOutputDesktop::GetPinMask() const -> uint32_t { return m_pinMask; }

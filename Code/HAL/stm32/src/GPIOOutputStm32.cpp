#include "GPIOOutputStm32.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputStm32::SetState(bool on) -> bool {
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

auto GPIOOutputStm32::GetState() -> bool {
    if (m_outputRegister == nullptr) {
        return false;
    }

    return *m_outputRegister & m_pinMask;
}

//---------------
// IGPIOOutputStm32
//---------------
auto GPIOOutputStm32::SetOutputRegister(volatile uint32_t* outputRegister) -> bool {
    if (outputRegister == nullptr) {
        return false;
    }

    m_outputRegister = outputRegister;

    return true;
}
auto GPIOOutputStm32::GetOutputRegister() const -> volatile const uint32_t* { return m_outputRegister; }

auto GPIOOutputStm32::SetPinMask(uint32_t pinMask) -> bool {
    if ((pinMask == 0) || (((pinMask & (pinMask - 1)) != 0))) {
        return false;
    }

    m_pinMask = pinMask;

    return true;
}
auto GPIOOutputStm32::GetPinMask() const -> uint32_t { return m_pinMask; }

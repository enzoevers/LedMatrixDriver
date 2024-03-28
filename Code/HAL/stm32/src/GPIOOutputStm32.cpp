#include "GPIOOutputStm32.h"

#include "ValueChecks.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputStm32::SetState(bool on) -> bool {
    if (m_gPIOOutputConfigStm32.pOutputRegister == nullptr) {
        return false;
    }

    if (on) {
        *m_gPIOOutputConfigStm32.pOutputRegister |= m_gPIOOutputConfigStm32.pinMask;
    } else {
        *m_gPIOOutputConfigStm32.pOutputRegister &= ~m_gPIOOutputConfigStm32.pinMask;
    }

    return true;
}

auto GPIOOutputStm32::GetState() -> bool {
    if (m_gPIOOutputConfigStm32.pOutputRegister == nullptr) {
        return false;
    }

    return *m_gPIOOutputConfigStm32.pOutputRegister & m_gPIOOutputConfigStm32.pinMask;
}

//---------------
// IGPIOOutputStm32
//---------------
auto GPIOOutputStm32::SetupConfiguration(const GPIOOutputConfigStm32&& gPIOOutputConfigstm32) -> bool {
    if ((gPIOOutputConfigstm32.pOutputRegister == nullptr) ||
        !ValueChecks::HasSingleBitSet(gPIOOutputConfigstm32.pinMask)) {
        return false;
    }

    m_gPIOOutputConfigStm32 = gPIOOutputConfigstm32;

    return true;
}
auto GPIOOutputStm32::GetConfiguration() -> const GPIOOutputConfigStm32& { return m_gPIOOutputConfigStm32; }

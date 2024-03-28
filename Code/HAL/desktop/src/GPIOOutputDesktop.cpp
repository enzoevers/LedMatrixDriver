#include "GPIOOutputDesktop.h"

#include <stdexcept>

#include "ValueChecks.h"

//---------------
// IGPIOOutput
//---------------
auto GPIOOutputDesktop::SetState(bool on) -> bool {
    if (m_gPIOOutputConfigDesktop.pOutputRegister == nullptr) {
        return false;
    }

    if (on) {
        *m_gPIOOutputConfigDesktop.pOutputRegister |= m_gPIOOutputConfigDesktop.pinMask;
    } else {
        *m_gPIOOutputConfigDesktop.pOutputRegister &= ~m_gPIOOutputConfigDesktop.pinMask;
    }

    return true;
}

auto GPIOOutputDesktop::GetState() -> bool {
    if (m_gPIOOutputConfigDesktop.pOutputRegister == nullptr) {
        return false;
    }

    return *m_gPIOOutputConfigDesktop.pOutputRegister & m_gPIOOutputConfigDesktop.pinMask;
}

//---------------
// IGPIOOutputDesktop
//---------------
auto GPIOOutputDesktop::SetupConfiguration(const GPIOOutputConfigDesktop&& gPIOOutputConfigDesktop) -> void {
    if (gPIOOutputConfigDesktop.pOutputRegister == nullptr) {
        throw std::invalid_argument("No nullptr allowed");
    }

    if (!ValueChecks::HasSingleBitSet(gPIOOutputConfigDesktop.pinMask)) {
        throw std::invalid_argument("Only 1 set bit allowed");
    }

    m_gPIOOutputConfigDesktop = gPIOOutputConfigDesktop;
}
auto GPIOOutputDesktop::GetConfiguration() -> const GPIOOutputConfigDesktop& { return m_gPIOOutputConfigDesktop; }

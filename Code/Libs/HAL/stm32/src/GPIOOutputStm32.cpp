#include "GPIOOutputStm32.h"

#include "ValueChecks.h"

namespace HAL::STM32 {

//---------------
// HAL::IGPIOOutput
//---------------
auto GPIOOutput::SetState(bool on) -> bool {
    if (m_gPIOOutputConfig.pOutputRegister == nullptr) {
        return false;
    }

    if (on) {
        *m_gPIOOutputConfig.pOutputRegister |= m_gPIOOutputConfig.pinMask;
    } else {
        *m_gPIOOutputConfig.pOutputRegister &= ~m_gPIOOutputConfig.pinMask;
    }

    return true;
}

auto GPIOOutput::GetState() -> bool {
    if (m_gPIOOutputConfig.pOutputRegister == nullptr) {
        return false;
    }

    return *m_gPIOOutputConfig.pOutputRegister & m_gPIOOutputConfig.pinMask;
}

//---------------
// HAL::STM32::IGPIOOutput
//---------------
auto GPIOOutput::SetupConfiguration(const GPIOOutputConfig&& gPIOOutputConfig) -> bool {
    if ((gPIOOutputConfig.pOutputRegister == nullptr) || !ValueChecks::HasSingleBitSet(gPIOOutputConfig.pinMask)) {
        return false;
    }

    m_gPIOOutputConfig = gPIOOutputConfig;

    return true;
}
auto GPIOOutput::GetConfiguration() -> const GPIOOutputConfig& { return m_gPIOOutputConfig; }

}  // namespace HAL::STM32
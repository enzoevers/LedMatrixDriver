#include "GPIOInputStm32.h"

#include "ValueChecks.h"

namespace HAL::STM32 {

GPIOInput::GPIOInput()
    : m_gPIOInputConfig({nullptr, 0, 0}), m_stablePinState(0), m_previousPinState(0), m_debounceCounter(0) {}

//---------------
// HAL::IGPIOInput
//---------------
auto GPIOInput::GetState() -> bool { return m_stablePinState; }

//---------------
// HAL::STM32::IGPIOInput
//---------------
auto GPIOInput::SetupConfiguration(const GPIOInputConfig&& gPIOInputConfig) -> bool {
    if ((gPIOInputConfig.pInputRegister == nullptr) || !ValueChecks::HasSingleBitSet(gPIOInputConfig.pinMask)) {
        return false;
    }

    m_gPIOInputConfig = gPIOInputConfig;

    return true;
}
auto GPIOInput::GetConfiguration() -> const GPIOInputConfig& { return m_gPIOInputConfig; }

auto GPIOInput::DebounceInterruptHandler() -> bool {
    if (m_gPIOInputConfig.pInputRegister == nullptr) {
        return false;
    }

    const bool currentPinReading = *m_gPIOInputConfig.pInputRegister & m_gPIOInputConfig.pinMask;

    if (currentPinReading == m_previousPinState) {
        ++m_debounceCounter;
    } else {
        m_debounceCounter = 0;
    }

    m_previousPinState = currentPinReading;

    if (m_debounceCounter >= m_gPIOInputConfig.debounceCount) {
        m_stablePinState = currentPinReading;
        m_debounceCounter = 0;
    }

    return true;
}

}  // namespace HAL::STM32
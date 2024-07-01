#include "DelayStm32.h"

#include <cmath>

#include "ValueChecks.h"
namespace HAL::STM32 {

//---------------
// HAL::IDelay
//---------------
auto Delay::SynchronousWait_us(uint32_t microseconds) -> bool {
    // Based on https://gist.github.com/MayaPosch/32710d2aac8c46cb6327479b203f0b27
    // Note that the actual counter enable signal CNT_EN is set 1 clock cycle after CEN

    // TODO: subtract (estimated) clock required for the code itself

    const auto prescaler = CalculateRequiredPrescaler(m_delayConfig.timerInputFrequencyInHertz, microseconds);
    if (prescaler == DELAY_NOT_POSSIBLE) {
        return false;
    }
    *m_delayConfig.pPrescalerRegister = prescaler;

    const auto autoReloadValue =
        CalculateRequiredClocks(m_delayConfig.timerInputFrequencyInHertz, prescaler, microseconds);
    *m_delayConfig.pAutoReloadRegister = autoReloadValue;

    *m_delayConfig.pStatusRegister = 0;
    *m_delayConfig.pControlRegister |= m_delayConfig.counterEnableMask;

    while (!(*m_delayConfig.pStatusRegister & m_delayConfig.updateInterruptMask))
        ;

    return true;
};

auto Delay::CalculateRequiredPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t microseconds) -> uint16_t {
    // Check if microseconds is too short
    const float microsecondsPerClock = 1000000.0f / timerInputFrequencyInHertz;
    if (microsecondsPerClock == 0) {
        // Higher clock than 1GHz not supported
        return DELAY_NOT_POSSIBLE;
    }

    if (microsecondsPerClock > microseconds) {
        return DELAY_NOT_POSSIBLE;
    }

    const auto maxArrValue = static_cast<uint32_t>(UINT16_MAX);

    const float minimumMicrosecondsPerClockRequired =
        static_cast<float>(microseconds) / static_cast<float>(maxArrValue);

    const auto requiredPrescaler =
        static_cast<uint32_t>(ceil(minimumMicrosecondsPerClockRequired / microsecondsPerClock));

    const auto prescalerRegisterValue = requiredPrescaler - 1;
    if (prescalerRegisterValue >= UINT16_MAX) {
        return DELAY_NOT_POSSIBLE;
    } else {
        return prescalerRegisterValue;
    }
}

auto Delay::CalculateRequiredClocks(uint32_t timerInputFrequencyInHertz, uint16_t prescaler, uint32_t microseconds)
    -> uint16_t {
    const float microsecondsPerClock =
        CalculateMicrosecondsPerClockWithPrescaler(timerInputFrequencyInHertz, prescaler);
    const uint16_t requiredClocks = microseconds / microsecondsPerClock;

    return requiredClocks;
}

auto Delay::CalculateMicrosecondsPerClockWithPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t prescaler)
    -> float {
    const auto clockAfterPrescaler = timerInputFrequencyInHertz / (static_cast<uint32_t>(prescaler) + 1);
    const float microsecondsPerClock = 1000000.0f / clockAfterPrescaler;

    return microsecondsPerClock;
}

//---------------
// HAL::STM32::IDelay
//---------------
auto Delay::SetupConfiguration(const DelayConfig&& delayConfig) -> bool {
    if (delayConfig.pStatusRegister == nullptr || delayConfig.pAutoReloadRegister == nullptr ||
        delayConfig.pControlRegister == nullptr || delayConfig.pPrescalerRegister == nullptr ||
        !ValueChecks::HasSingleBitSet(delayConfig.updateInterruptMask) ||
        !ValueChecks::HasSingleBitSet(delayConfig.counterEnableMask)) {
        return false;
    }

    m_delayConfig = delayConfig;
    return true;
}

auto Delay::GetConfiguration() -> const DelayConfig& { return m_delayConfig; }

}  // namespace HAL::STM32
#include "DelayStm32.h"

#include <cmath>

#include "ValueChecks.h"
//---------------
// IDelay
//---------------
auto DelayStm32::SynchronousWait_us(uint32_t microseconds) -> bool {
    // Based on https://gist.github.com/MayaPosch/32710d2aac8c46cb6327479b203f0b27
    // Note that the actual counter enable signal CNT_EN is set 1 clock cycle after CEN

    // TODO: subtract (estimated) clock required for the code itself

    const auto prescaler = CalculateRequiredPrescaler(m_delayConfigstm32.timerInputFrequencyInHertz, microseconds);
    if (prescaler == DELAY_NOT_POSSIBLE) {
        return false;
    }
    *m_delayConfigstm32.pPrescalerRegister = prescaler;

    const auto autoReloadValue =
        CalculateRequiredClocks(m_delayConfigstm32.timerInputFrequencyInHertz, prescaler, microseconds);
    *m_delayConfigstm32.pAutoReloadRegister = autoReloadValue;

    *m_delayConfigstm32.pStatusRegister = 0;
    *m_delayConfigstm32.pControlRegister |= m_delayConfigstm32.counterEnableMask;

    while (!(*m_delayConfigstm32.pStatusRegister & m_delayConfigstm32.updateInterruptMask)) {
        // Empty loop
    }

    return true;
};

auto DelayStm32::CalculateRequiredPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t microseconds) -> uint16_t {
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

auto DelayStm32::CalculateRequiredClocks(uint32_t timerInputFrequencyInHertz, uint16_t prescaler, uint32_t microseconds)
    -> uint16_t {
    const float microsecondsPerClock =
        CalculateMicrosecondsPerClockWithPrescaler(timerInputFrequencyInHertz, prescaler);
    const uint16_t requiredClocks = microseconds / microsecondsPerClock;

    return requiredClocks;
}

auto DelayStm32::CalculateMicrosecondsPerClockWithPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t prescaler)
    -> float {
    const auto clockAfterPrescaler = timerInputFrequencyInHertz / (static_cast<uint32_t>(prescaler) + 1);
    const float microsecondsPerClock = 1000000.0f / clockAfterPrescaler;

    return microsecondsPerClock;
}

//---------------
// IDelayStm32
//---------------
auto DelayStm32::SetupConfiguration(const DelayConfigstm32&& delayConfigstm32) -> bool {
    if (delayConfigstm32.pStatusRegister == nullptr || delayConfigstm32.pAutoReloadRegister == nullptr ||
        delayConfigstm32.pControlRegister == nullptr || delayConfigstm32.pPrescalerRegister == nullptr ||
        !ValueChecks::HasSingleBitSet(delayConfigstm32.updateInterruptMask) ||
        !ValueChecks::HasSingleBitSet(delayConfigstm32.counterEnableMask)) {
        return false;
    }

    m_delayConfigstm32 = delayConfigstm32;
    return true;
}

auto DelayStm32::GetConfiguration() -> const DelayConfigstm32& { return m_delayConfigstm32; }
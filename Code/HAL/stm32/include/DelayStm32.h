#pragma once

#include "IDelay.h"
#include "IDelayStm32.h"

namespace HAL::STM32 {
/*!
 * \class Delay
 * \brief Use the stm32 timer register to wait a certain amount of microseconds.
 */
class Delay : public HAL::IDelay, public HAL::STM32::IDelay {
   public:
    //---------------
    // HAL::IDelay
    //---------------
    auto SynchronousWait_us(uint32_t microseconds) -> bool override;

    //---------------
    // HAL::STM32::IDelay
    //---------------
    auto SetupConfiguration(const DelayConfig&& delayConfig) -> bool override;
    auto GetConfiguration() -> const DelayConfig& override;

   private:
    /*! \returns UINT16_MAX if the requested delay is not possible. Otherwise return the required prescaler */
    static auto CalculateRequiredPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t microseconds) -> uint16_t;
    static auto CalculateRequiredClocks(uint32_t timerInputFrequencyInHertz, uint16_t prescaler, uint32_t microseconds)
        -> uint16_t;
    static auto CalculateMicrosecondsPerClockWithPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t prescaler)
        -> float;

    DelayConfig m_delayConfig;

    static const auto DELAY_NOT_POSSIBLE = UINT16_MAX;
};

}  // namespace HAL::STM32
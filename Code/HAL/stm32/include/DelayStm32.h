#pragma once

#include "IDelay.h"
#include "IDelayStm32.h"

/*!
 * \class DelayStm32
 * \brief Use the stm32 timer register to wait a certain amount of microseconds.
 */
class DelayStm32 : public IDelay, public IDelayStm32 {
   public:
    //---------------
    // IDelay
    //---------------
    auto SynchronousWait_us(uint32_t microseconds) -> bool override;

    //---------------
    // IDelayStm32
    //---------------
    auto SetupConfiguration(const DelayConfigstm32&& delayConfigstm32) -> bool override;
    auto GetConfiguration() -> const DelayConfigstm32& override;

   private:
    /*! \returns UINT16_MAX if the requested delay is not possible. Otherwise return the required prescaler */
    static auto CalculateRequiredPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t microseconds) -> uint16_t;
    static auto CalculateRequiredClocks(uint32_t timerInputFrequencyInHertz, uint16_t prescaler, uint32_t microseconds)
        -> uint16_t;
    static auto CalculateMicrosecondsPerClockWithPrescaler(uint32_t timerInputFrequencyInHertz, uint32_t prescaler)
        -> float;

    DelayConfigstm32 m_delayConfigstm32;

    static const auto DELAY_NOT_POSSIBLE = UINT16_MAX;
};
#pragma once

#include "DelayConfigStm32.h"

namespace HAL::STM32 {

class IDelay {
   public:
    virtual ~IDelay() = default;

    virtual auto SetupConfiguration(const DelayConfig&& delayConfig) -> bool = 0;
    virtual auto GetConfiguration() -> const DelayConfig& = 0;
};

}  // namespace HAL::STM32
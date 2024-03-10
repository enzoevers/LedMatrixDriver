#pragma once

#include "DelayConfigStm32.h"

class IDelayStm32 {
   public:
    virtual ~IDelayStm32() = default;

    virtual auto SetupConfiguration(const DelayConfigstm32&& delayConfigstm32) -> bool = 0;
    virtual auto GetConfiguration() -> const DelayConfigstm32& = 0;
};
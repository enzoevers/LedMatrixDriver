#pragma once

#include "DateTimeConfigStm32.h"

namespace HAL::STM32 {

class IDateTime {
   public:
    virtual ~IDateTime() = default;

    virtual auto SetConfig(const DateTimeConfig&& dateTimeConfig) -> bool = 0;
    virtual auto GetConfig() -> const DateTimeConfig& = 0;
};

}  // namespace HAL::STM32
#pragma once

#include "DateTimeConfigStm32.h"

class IDateTimeStm32 {
   public:
    virtual ~IDateTimeStm32() = default;

    virtual auto SetConfig(const DateTimeConfigStm32&& dateTimeConfigStm32) -> bool = 0;
    virtual auto GetConfig() -> const DateTimeConfigStm32& = 0;
};
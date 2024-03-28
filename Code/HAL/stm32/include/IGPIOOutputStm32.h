#pragma once

#include "GPIOOutputConfigStm32.h"

class IGPIOOutputStm32 {
   public:
    virtual ~IGPIOOutputStm32() = default;

    // \note If GPIOOutputConfigStm32.outputRegister is a nullptr, the function returns false.
    // Otherwise it returns true.
    // GPIOOutputConfigStm32.pinMask is allowed to only have 1 bit set. If this is not the case,
    // the function returns false. It returns true otherwise.
    virtual auto SetupConfiguration(const GPIOOutputConfigStm32&& gPIOOutputConfigStm32) -> bool = 0;
    virtual auto GetConfiguration() -> const GPIOOutputConfigStm32& = 0;
};
#pragma once

#include "GPIOOutputConfigStm32.h"

namespace HAL::STM32 {

class IGPIOOutput {
   public:
    virtual ~IGPIOOutput() = default;

    // \note If GPIOOutputConfig.outputRegister is a nullptr, the function returns false.
    // Otherwise it returns true.
    // GPIOOutputConfig.pinMask is allowed to only have 1 bit set. If this is not the case,
    // the function returns false. It returns true otherwise.
    virtual auto SetupConfiguration(const GPIOOutputConfig&& gPIOOutputConfig) -> bool = 0;
    virtual auto GetConfiguration() -> const GPIOOutputConfig& = 0;
};

}  // namespace HAL::STM32
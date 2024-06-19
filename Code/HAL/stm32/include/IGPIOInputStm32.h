#pragma once

#include "GPIOInputConfigStm32.h"

namespace HAL::STM32 {

class IGPIOInput {
   public:
    virtual ~IGPIOInput() = default;

    virtual auto SetupConfiguration(const GPIOInputConfig&& gPIOInputConfig) -> bool = 0;
    virtual auto GetConfiguration() -> const GPIOInputConfig& = 0;

    /*
    \brief  This function must be called in the timer IRQ handler. It is assumed that
            period of the timer is set appropriately to the debounce time.
            The config that is passed to this function contains the amount of times that the
            pin state should be the same before it is considered stable (GPIOInputConfig.debounceCount).
    */
    virtual auto DebounceInterruptHandler() -> bool = 0;
};

}  // namespace HAL::STM32
#pragma once

#include "GPIOOutputConfigDesktop.h"

class IGPIOOutputDesktop {
   public:
    virtual ~IGPIOOutputDesktop() = default;

    // \note If GPIOOutputConfigDesktop.outputRegister is a nullptr, std::invalid_argument("No nullptr allowed") is
    // thrown. GPIOOutputConfigDesktop.pinMask is allowed to only have 1 bit set. If this is not the case, it throws
    // std::invalid_argument("Only 1 set bit allowed").
    virtual auto SetupConfiguration(const GPIOOutputConfigDesktop&& gPIOOutputConfigDesktop) -> void = 0;
    virtual auto GetConfiguration() -> const GPIOOutputConfigDesktop& = 0;
};
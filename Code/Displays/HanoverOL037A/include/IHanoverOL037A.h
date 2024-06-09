#pragma once

#include "HanoverOL037A_GPIOInterface.h"
#include "IDelay.h"

class IHanoverOL037A {
   public:
    virtual ~IHanoverOL037A() = default;

    virtual auto SetGPIOInterface(const HanoverOL037A_GPIOInterface& gpioInterface) -> bool = 0;
    virtual auto GetGPIOInterface() const -> const HanoverOL037A_GPIOInterface& = 0;
    virtual auto SetDelayManager(HAL::IDelay* const delayManager) -> void = 0;
    virtual auto GetDelayManager() const -> HAL::IDelay* = 0;
};
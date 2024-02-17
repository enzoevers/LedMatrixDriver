#pragma once

#include <stdint.h>

class IGPIOOutputStm32 {
   public:
    virtual ~IGPIOOutputStm32() = default;

    virtual auto SetOutputRegister(uint32_t* outputRegister) -> void = 0;
    virtual auto GetOutputRegister() const -> const uint32_t* = 0;

    virtual auto SetPinMask(uint32_t pinMask) -> void = 0;
    virtual auto GetPinMask() const -> uint32_t = 0;
};
#pragma once

#include <stdint.h>

class IGPIOOutputStm32 {
   public:
    virtual ~IGPIOOutputStm32() = default;

    // \note If outputRegister is a nullptr, the function returns false. Otherwise it returns true.
    virtual auto SetOutputRegister(volatile uint32_t* outputRegister) -> bool = 0;
    virtual auto GetOutputRegister() const -> volatile const uint32_t* = 0;

    // \note pinMask is allowed to only have 1 bit set. If this is not the case,
    // the function returns false. It returns true otherwise.
    virtual auto SetPinMask(uint32_t pinMask) -> bool = 0;
    virtual auto GetPinMask() const -> uint32_t = 0;
};
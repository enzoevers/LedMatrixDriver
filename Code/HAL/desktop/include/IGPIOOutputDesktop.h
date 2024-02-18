#pragma once

#include <stdint.h>

class IGPIOOutputDesktop {
   public:
    virtual ~IGPIOOutputDesktop() = default;

    // \note If outputRegister is a nullptr, std::invalid_argument("No nullptr allowed") is thrown.
    virtual auto SetOutputRegister(uint32_t* outputRegister) -> void = 0;
    virtual auto GetOutputRegister() const -> const uint32_t* = 0;

    // \note pinMask is allowed to only have 1 bit set. If this is not the case,
    // it throws std::invalid_argument("Only 1 set bit allowed").
    virtual auto SetPinMask(uint32_t pinMask) -> void = 0;
    virtual auto GetPinMask() const -> uint32_t = 0;
};
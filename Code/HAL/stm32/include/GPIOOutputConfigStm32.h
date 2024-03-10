#pragma once

#include <stdint.h>

struct GPIOOutputConfigStm32 {
    volatile uint32_t* pOutputRegister;
    uint32_t pinMask;

    auto operator==(const GPIOOutputConfigStm32& rhs) const -> bool {
        return (this->pOutputRegister == rhs.pOutputRegister && this->pinMask == rhs.pinMask);
    }
};
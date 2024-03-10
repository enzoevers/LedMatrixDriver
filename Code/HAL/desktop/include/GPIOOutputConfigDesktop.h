#pragma once

#include <stdint.h>

struct GPIOOutputConfigDesktop {
    volatile uint32_t* pOutputRegister;
    uint32_t pinMask;

    auto operator==(const GPIOOutputConfigDesktop& rhs) const -> bool {
        return (this->pOutputRegister == rhs.pOutputRegister && this->pinMask == rhs.pinMask);
    }
};
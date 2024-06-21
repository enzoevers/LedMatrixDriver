#pragma once

#include <stdint.h>

namespace HAL::STM32 {

struct GPIOOutputConfig {
    volatile uint32_t* pOutputRegister;
    uint32_t pinMask;

    auto operator==(const GPIOOutputConfig& rhs) const -> bool {
        return (this->pOutputRegister == rhs.pOutputRegister && this->pinMask == rhs.pinMask);
    }
};

}  // namespace HAL::STM32
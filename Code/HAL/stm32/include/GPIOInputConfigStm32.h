#pragma once

#include <stdint.h>

namespace HAL::STM32 {

struct GPIOInputConfig {
    volatile uint32_t* pInputRegister;
    uint32_t pinMask;
    uint8_t debounceCount;

    auto operator==(const GPIOInputConfig& rhs) const -> bool {
        return (this->pInputRegister == rhs.pInputRegister && this->pinMask == rhs.pinMask &&
                this->debounceCount == rhs.debounceCount);
    }
};

}  // namespace HAL::STM32
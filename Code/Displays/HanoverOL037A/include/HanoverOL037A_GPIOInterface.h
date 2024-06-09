#pragma once

#include "IGPIOOutput.h"

struct HanoverOL037A_GPIOInterface {
    HAL::IGPIOOutput* clk;
    HAL::IGPIOOutput* clkEn;
    HAL::IGPIOOutput* clkSelEn;
    HAL::IGPIOOutput* data;
    HAL::IGPIOOutput* latch;
    HAL::IGPIOOutput* ledOE;  // TODO: later to be replaces with IPWMOutput&

    auto ContainsNullptr() const -> bool {
        return (clk == nullptr || clkEn == nullptr || clkSelEn == nullptr || data == nullptr || latch == nullptr ||
                ledOE == nullptr);
    }

    auto operator==(const HanoverOL037A_GPIOInterface& rhs) const -> bool {
        return (this->clk == rhs.clk && this->clkEn == rhs.clkEn && this->clkSelEn == rhs.clkSelEn &&
                this->data == rhs.data && this->latch == rhs.latch && this->ledOE == rhs.ledOE);
    }
};

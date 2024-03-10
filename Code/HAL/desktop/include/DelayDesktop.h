#pragma once

#include "IDelay.h"

class DelayDesktop : public IDelay {
   public:
    //---------------
    // IDelay
    //---------------
    auto SynchronousWait_us(uint32_t microseconds) -> bool override;
};
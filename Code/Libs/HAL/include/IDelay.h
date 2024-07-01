#pragma once

#include <stdint.h>

namespace HAL {

class IDelay {
   public:
    virtual ~IDelay() = default;

    /*!
     * \brief Halting the process by, effectively looping for a certain amount of microseconds.
     * \returns True is the waiting was succesful. False is the microseconds amount is too short or long.
     */
    virtual auto SynchronousWait_us(uint32_t microseconds) -> bool = 0;
};

}  // namespace HAL
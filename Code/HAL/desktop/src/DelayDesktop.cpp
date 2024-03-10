#include "DelayDesktop.h"

#include <unistd.h>

//---------------
// IDelay
//---------------
auto DelayDesktop::SynchronousWait_us(uint32_t microseconds) -> bool {
    usleep(microseconds);
    return true;
};
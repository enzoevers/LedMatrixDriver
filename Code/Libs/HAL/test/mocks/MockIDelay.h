#pragma once

#include <gmock/gmock.h>

#include "IDelay.h"

namespace HAL::Mock {

class MockIDelay : public IDelay {
   public:
    ~MockIDelay() override = default;

    MOCK_METHOD(bool, SynchronousWait_us, (uint32_t microseconds), (override));
};

}  // namespace HAL::Mock
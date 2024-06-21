#pragma once

#include <gmock/gmock.h>

#include "IGPIOOutput.h"

namespace HAL::Mock {

class MockIGPIOOutput : public IGPIOOutput {
   public:
    ~MockIGPIOOutput() override = default;

    MOCK_METHOD(bool, SetState, (bool on), (override));
    MOCK_METHOD(bool, GetState, (), (override));
};

}  // namespace HAL::Mock
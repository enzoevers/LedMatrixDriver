#pragma once

#include <gmock/gmock.h>

#include "IGPIOInput.h"

namespace HAL::Mock {

class MockIGPIOInput : public IGPIOInput {
   public:
    ~MockIGPIOInput() override = default;

    MOCK_METHOD(bool, GetState, (), (override));
};

}  // namespace HAL::Mock
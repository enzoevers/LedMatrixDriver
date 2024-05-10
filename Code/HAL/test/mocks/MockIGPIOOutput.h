#pragma once

#include <gmock/gmock.h>

#include "IGPIOOutput.h"

class MockIGPIOOutput : public IGPIOOutput {
   public:
    ~MockIGPIOOutput() override = default;

    MOCK_METHOD(bool, SetState, (bool on), (override));
    MOCK_METHOD(bool, GetState, (), (override));
};
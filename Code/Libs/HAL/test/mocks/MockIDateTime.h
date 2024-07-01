#pragma once

#include <gmock/gmock.h>

#include "IDateTime.h"

namespace HAL::Mock {

class MockIDateTime : public IDateTime {
   public:
    ~MockIDateTime() override = default;

    MOCK_METHOD(Common::Types::Time, GetTime, (), (override));
    MOCK_METHOD(bool, SetTime, (const Common::Types::Time& time), (override));
    MOCK_METHOD(Common::Types::Date, GetDate, (), (override));
    MOCK_METHOD(bool, SetDate, (const Common::Types::Date& date), (override));
    MOCK_METHOD(Common::Types::DateTime, GetDateTime, (), (override));
    MOCK_METHOD(bool, SetDateTime, (const Common::Types::DateTime& dateTime), (override));
};

}  // namespace HAL::Mock

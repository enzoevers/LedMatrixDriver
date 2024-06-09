#pragma once

#include "DateTimeData.h"

namespace HAL {
class IDateTime {
   public:
    virtual ~IDateTime() = default;

    virtual auto GetTime() -> Types::Time = 0;
    virtual auto SetTime(const Types::Time& time) -> bool = 0;
    virtual auto GetDate() -> Types::Date = 0;
    virtual auto SetDate(const Types::Date& date) -> bool = 0;
    virtual auto GetDateTime() -> Types::DateTime = 0;
    virtual auto SetDateTime(const Types::DateTime& dateTime) -> bool = 0;
};

}  // namespace HAL
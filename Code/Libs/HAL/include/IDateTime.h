#pragma once

#include "Types/DateTimeData.h"

namespace HAL {
class IDateTime {
   public:
    virtual ~IDateTime() = default;

    virtual auto GetTime() -> Common::Types::Time = 0;
    virtual auto SetTime(const Common::Types::Time& time) -> bool = 0;
    virtual auto GetDate() -> Common::Types::Date = 0;
    virtual auto SetDate(const Common::Types::Date& date) -> bool = 0;
    virtual auto GetDateTime() -> Common::Types::DateTime = 0;
    virtual auto SetDateTime(const Common::Types::DateTime& dateTime) -> bool = 0;
};

}  // namespace HAL
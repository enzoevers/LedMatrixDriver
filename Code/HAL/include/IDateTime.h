#pragma once

#include "DateTimeData.h"

class IDateTime {
   public:
    virtual ~IDateTime() = default;

    virtual auto GetTime() -> Time = 0;
    virtual auto SetTime(const Time& time) -> bool = 0;
    virtual auto GetDate() -> Date = 0;
    virtual auto SetDate(const Date& date) -> bool = 0;
    virtual auto GetDateTime() -> DateTime = 0;
    virtual auto SetDateTime(const DateTime& dateTime) -> bool = 0;
};
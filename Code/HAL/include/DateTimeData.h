#pragma once

#include <stdint.h>

struct Time {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;

    auto operator==(const Time& other) const -> bool {
        return hours == other.hours && minutes == other.minutes && seconds == other.seconds &&
               milliseconds == other.milliseconds;
    }
};

enum class Weekday { Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

struct Date {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    Weekday weekday;

    auto operator==(const Date& other) const -> bool {
        return year == other.year && month == other.month && day == other.day && weekday == other.weekday;
    }
};

struct DateTime {
    Time time;
    Date date;

    auto operator==(const DateTime& other) const -> bool { return time == other.time && date == other.date; }
};
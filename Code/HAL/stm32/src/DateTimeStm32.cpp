#include "DateTimeStm32.h"

namespace HAL::STM32 {

DateTime::DateTime() : m_dateTimeConfig({nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0}) {}

//---------------
// HAL::IDateTime
//---------------

auto DateTime::GetTime() -> Types::Time {
    auto time = Types::Time{.hours = 0, .minutes = 0, .seconds = 0, .milliseconds = 0};

    if (m_dateTimeConfig.ContainsNullptr()) {
        return time;
    }

    const uint32_t timeReg = *m_dateTimeConfig.RTC_TR;

    const uint8_t hoursTen = (timeReg & (0b11 << 20)) >> 20;
    const uint8_t hoursSingle = (timeReg & (0b1111 << 16)) >> 16;
    const uint8_t minutesTen = (timeReg & (0b111 << 12)) >> 12;
    const uint8_t minutesSingle = (timeReg & (0b1111 << 8)) >> 8;
    const uint8_t secondsTen = (timeReg & (0b111 << 4)) >> 4;
    const uint8_t secondsSingle = timeReg & 0b1111;

    time.hours = hoursTen * 10 + hoursSingle;
    time.minutes = minutesTen * 10 + minutesSingle;
    time.seconds = secondsTen * 10 + secondsSingle;

    return time;
}

auto DateTime::SetTime(const Types::Time& time) -> bool {
    if (m_dateTimeConfig.ContainsNullptr()) {
        return false;
    }

    UnlockRtcRegisters();
    EnterInitializeMode();

    InternalSetTime(time);

    ExitInitializeMode();
    LockRtcRegisters();

    return true;
}

auto DateTime::GetDate() -> Types::Date {
    auto date = Types::Date{.year = 0, .month = 0, .day = 0, .weekday = Types::Weekday::Monday};

    if (m_dateTimeConfig.ContainsNullptr()) {
        return date;
    }

    const uint32_t dateReg = *m_dateTimeConfig.RTC_DR;

    const uint16_t yearTens = (dateReg & (0b1111 << 20)) >> 20;
    const uint16_t yearSingles = (dateReg & (0b1111 << 16)) >> 16;
    const uint8_t weekdayInt = (dateReg & (0b111 << 13)) >> 13;
    const uint8_t monthTens = (dateReg & (0b1 << 12)) >> 12;
    const uint8_t monthSingles = (dateReg & (0b1111 << 8)) >> 8;
    const uint8_t dayTens = (dateReg & (0b11 << 4)) >> 4;
    const uint8_t daySingles = dateReg & 0b1111;

    constexpr uint8_t multiplier = 10;

    date = {
        .year = static_cast<uint16_t>(m_yearBase + (yearTens * multiplier) + yearSingles),
        .month = static_cast<uint8_t>((monthTens * multiplier) + monthSingles),
        .day = static_cast<uint8_t>((dayTens * multiplier) + daySingles),
        .weekday = static_cast<Types::Weekday>(weekdayInt),
    };

    return date;
}

auto DateTime::SetDate(const Types::Date& date) -> bool {
    if (m_dateTimeConfig.ContainsNullptr()) {
        return false;
    }

    UnlockRtcRegisters();
    EnterInitializeMode();

    InternalSetDate(date);

    ExitInitializeMode();
    LockRtcRegisters();

    return true;
}

auto DateTime::GetDateTime() -> Types::DateTime {
    auto dateTime =
        Types::DateTime{.time = Types::Time{.hours = 0, .minutes = 0, .seconds = 0, .milliseconds = 0},
                        .date = Types::Date{.year = 0, .month = 0, .day = 0, .weekday = Types::Weekday::Monday}};

    if (m_dateTimeConfig.ContainsNullptr()) {
        return dateTime;
    }

    dateTime = {.time = GetTime(), .date = GetDate()};

    return dateTime;
}

auto DateTime::SetDateTime(const Types::DateTime& dateTime) -> bool {
    if (m_dateTimeConfig.ContainsNullptr()) {
        return false;
    }

    UnlockRtcRegisters();
    EnterInitializeMode();

    InternalSetTime(dateTime.time);
    InternalSetDate(dateTime.date);

    ExitInitializeMode();
    LockRtcRegisters();

    return true;
}

//---------------
// HAL::STM32::IDateTime
//---------------

auto DateTime::SetConfig(const DateTimeConfig&& dateTimeConfig) -> bool {
    if (dateTimeConfig.ContainsNullptr()) {
        return false;
    }

    m_dateTimeConfig = dateTimeConfig;

    return true;
}

auto DateTime::GetConfig() -> const DateTimeConfig& { return m_dateTimeConfig; }

//---------------
// Private
//---------------

auto DateTime::InternalSetTime(const Types::Time& time) -> void {
    const uint8_t hourTens = time.hours / 10;
    const uint8_t hourSingles = time.hours - (hourTens * 10);
    const uint8_t minutesTens = time.minutes / 10;
    const uint8_t minutesSingles = time.minutes - (minutesTens * 10);
    const uint8_t secondsTens = time.seconds / 10;
    const uint8_t secondsSingles = time.seconds - (secondsTens * 10);

    *m_dateTimeConfig.RTC_CR &= ~m_dateTimeConfig.RTC_CR_FMF_pos;

    // Keep PM bit at 0 for 24 hour mode
    *m_dateTimeConfig.RTC_TR = (hourTens << 20) | (hourSingles << 16) | (minutesTens << 12) | (minutesSingles << 8) |
                               (secondsTens << 4) | secondsSingles;
}

auto DateTime::InternalSetDate(const Types::Date& date) -> void {
    m_yearBase = (date.year / 100) * 100;
    const uint8_t yearTens = (date.year - m_yearBase) / 10;
    const uint8_t yearSingles = (date.year - m_yearBase) - (yearTens * 10);
    const uint8_t monthTens = date.month / 10;
    const uint8_t monthSingles = date.month - (monthTens * 10);
    const uint8_t dayTens = date.day / 10;
    const uint8_t daySingles = date.day - (dayTens * 10);
    const uint8_t weekdayInt = static_cast<uint8_t>(date.weekday);

    *m_dateTimeConfig.RTC_DR = (yearTens << 20) | (yearSingles << 16) | (weekdayInt << 13) | (monthTens << 12) |
                               (monthSingles << 8) | (dayTens << 4) | daySingles;
}

auto DateTime::UnlockRtcRegisters() -> void {
    *m_dateTimeConfig.PWR_CR |= 0b1 << m_dateTimeConfig.PWR_CR_DBP_pos;
    *m_dateTimeConfig.RTC_WPR = 0xCA;
    *m_dateTimeConfig.RTC_WPR = 0x53;
}

auto DateTime::LockRtcRegisters() -> void {
    *m_dateTimeConfig.PWR_CR &= ~(0b1 << m_dateTimeConfig.PWR_CR_DBP_pos);
    *m_dateTimeConfig.RTC_WPR = 0;
}

auto DateTime::EnterInitializeMode() -> void {
    *m_dateTimeConfig.RTC_ISR |= 0b1 << m_dateTimeConfig.RTC_ISR_INIT_pos;
    while (!(*m_dateTimeConfig.RTC_ISR & (0b1 << m_dateTimeConfig.RTC_ISR_INITF_pos)))
        ;
}
auto DateTime::ExitInitializeMode() -> void {
    *m_dateTimeConfig.RTC_ISR &= ~(0b1 << m_dateTimeConfig.RTC_ISR_INIT_pos);
}

}  // namespace HAL::STM32
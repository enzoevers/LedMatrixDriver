#include "DateTimeStm32.h"

DateTimeStm32::DateTimeStm32()
    : m_dateTimeConfigStm32({nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0}) {}

//---------------
// ITime
//---------------

auto DateTimeStm32::GetTime() -> Time {
    auto time = Time{.hours = 0, .minutes = 0, .seconds = 0, .milliseconds = 0};

    if (m_dateTimeConfigStm32.ContainsNullptr()) {
        return time;
    }

    const uint32_t timeReg = *m_dateTimeConfigStm32.RTC_TR;

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

auto DateTimeStm32::SetTime(const Time& time) -> bool {
    if (m_dateTimeConfigStm32.ContainsNullptr()) {
        return false;
    }

    UnlockRtcRegisters();
    EnterInitializeMode();

    InternalSetTime(time);

    ExitInitializeMode();
    LockRtcRegisters();

    return true;
}

auto DateTimeStm32::GetDate() -> Date {
    auto date = Date{.year = 0, .month = 0, .day = 0, .weekday = Weekday::Monday};

    if (m_dateTimeConfigStm32.ContainsNullptr()) {
        return date;
    }

    const uint32_t dateReg = *m_dateTimeConfigStm32.RTC_DR;

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
        .weekday = static_cast<Weekday>(weekdayInt),
    };

    return date;
}

auto DateTimeStm32::SetDate(const Date& date) -> bool {
    if (m_dateTimeConfigStm32.ContainsNullptr()) {
        return false;
    }

    UnlockRtcRegisters();
    EnterInitializeMode();

    InternalSetDate(date);

    ExitInitializeMode();
    LockRtcRegisters();

    return true;
}

auto DateTimeStm32::GetDateTime() -> DateTime {
    auto dateTime = DateTime{.time = Time{.hours = 0, .minutes = 0, .seconds = 0, .milliseconds = 0},
                             .date = Date{.year = 0, .month = 0, .day = 0, .weekday = Weekday::Monday}};

    if (m_dateTimeConfigStm32.ContainsNullptr()) {
        return dateTime;
    }

    dateTime = {.time = GetTime(), .date = GetDate()};

    return dateTime;
}

auto DateTimeStm32::SetDateTime(const DateTime& dateTime) -> bool {
    if (m_dateTimeConfigStm32.ContainsNullptr()) {
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
// ITimeStm32
//---------------

auto DateTimeStm32::SetConfig(const DateTimeConfigStm32&& dateTimeConfigStm32) -> bool {
    if (dateTimeConfigStm32.ContainsNullptr()) {
        return false;
    }

    m_dateTimeConfigStm32 = dateTimeConfigStm32;

    return true;
}

auto DateTimeStm32::GetConfig() -> const DateTimeConfigStm32& { return m_dateTimeConfigStm32; }

//---------------
// Private
//---------------

auto DateTimeStm32::InternalSetTime(const Time& time) -> void {
    const uint8_t hourTens = time.hours / 10;
    const uint8_t hourSingles = time.hours - (hourTens * 10);
    const uint8_t minutesTens = time.minutes / 10;
    const uint8_t minutesSingles = time.minutes - (minutesTens * 10);
    const uint8_t secondsTens = time.seconds / 10;
    const uint8_t secondsSingles = time.seconds - (secondsTens * 10);

    *m_dateTimeConfigStm32.RTC_CR &= ~m_dateTimeConfigStm32.RTC_CR_FMF_pos;

    // Keep PM bit at 0 for 24 hour mode
    *m_dateTimeConfigStm32.RTC_TR = (hourTens << 20) | (hourSingles << 16) | (minutesTens << 12) |
                                    (minutesSingles << 8) | (secondsTens << 4) | secondsSingles;
}

auto DateTimeStm32::InternalSetDate(const Date& date) -> void {
    m_yearBase = (date.year / 100) * 100;
    const uint8_t yearTens = (date.year - m_yearBase) / 10;
    const uint8_t yearSingles = (date.year - m_yearBase) - (yearTens * 10);
    const uint8_t monthTens = date.month / 10;
    const uint8_t monthSingles = date.month - (monthTens * 10);
    const uint8_t dayTens = date.day / 10;
    const uint8_t daySingles = date.day - (dayTens * 10);
    const uint8_t weekdayInt = static_cast<uint8_t>(date.weekday);

    *m_dateTimeConfigStm32.RTC_DR = (yearTens << 20) | (yearSingles << 16) | (weekdayInt << 13) | (monthTens << 12) |
                                    (monthSingles << 8) | (dayTens << 4) | daySingles;
}

auto DateTimeStm32::UnlockRtcRegisters() -> void {
    *m_dateTimeConfigStm32.PWR_CR |= 0b1 << m_dateTimeConfigStm32.PWR_CR_DBP_pos;
    *m_dateTimeConfigStm32.RTC_WPR = 0xCA;
    *m_dateTimeConfigStm32.RTC_WPR = 0x53;
}

auto DateTimeStm32::LockRtcRegisters() -> void {
    *m_dateTimeConfigStm32.PWR_CR &= ~(0b1 << m_dateTimeConfigStm32.PWR_CR_DBP_pos);
    *m_dateTimeConfigStm32.RTC_WPR = 0;
}

auto DateTimeStm32::EnterInitializeMode() -> void {
    *m_dateTimeConfigStm32.RTC_ISR |= 0b1 << m_dateTimeConfigStm32.RTC_ISR_INIT_pos;
    while (!(*m_dateTimeConfigStm32.RTC_ISR & (0b1 << m_dateTimeConfigStm32.RTC_ISR_INITF_pos)))
        ;
}
auto DateTimeStm32::ExitInitializeMode() -> void {
    *m_dateTimeConfigStm32.RTC_ISR &= ~(0b1 << m_dateTimeConfigStm32.RTC_ISR_INIT_pos);
}
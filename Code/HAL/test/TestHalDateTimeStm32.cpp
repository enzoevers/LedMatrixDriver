#include "DateTimeStm32.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace DateTimeStm32Testing {

class FixtureDateTimeStm32 : public Test {
   public:
    FixtureDateTimeStm32()
        : m_dateTimeConfigStm32({.PWR_CR = &PWR_CR,
                                 .RTC_WPR = &RTC_WPR,
                                 .RTC_ISR = &RTC_ISR,
                                 .RTC_CR = &RTC_CR,
                                 .RTC_TR = &RTC_TR,
                                 .RTC_DR = &RTC_DR,
                                 .RTC_SSR = &RTC_SSR,
                                 .PWR_CR_DBP_pos = PWR_CR_DBP_pos,
                                 .RTC_ISR_INIT_pos = RTC_ISR_INIT_pos,
                                 .RTC_ISR_INITF_pos = RTC_ISR_INITF_pos,
                                 .RTC_CR_FMF_pos = RTC_CR_FMF_pos}),
          m_dateTimeStm32(std::make_unique<DateTimeStm32>()) {
        // Always have the register in init mode
        RTC_ISR |= 0b1 << RTC_ISR_INITF_pos;
    }

    DateTimeConfigStm32 m_dateTimeConfigStm32;

    uint32_t PWR_CR = 0;
    uint32_t RTC_WPR = 0;
    uint32_t RTC_ISR = 0;
    uint32_t RTC_CR = 0;
    uint32_t RTC_TR = 0;
    uint32_t RTC_DR = 0;
    uint32_t RTC_SSR = 0;
    uint32_t PWR_CR_DBP_pos = 0b1 << 8;
    uint32_t RTC_ISR_INIT_pos = 0b1 << 7;
    uint32_t RTC_ISR_INITF_pos = 0b1 << 6;
    uint32_t RTC_CR_FMF_pos = 0b1 << 6;

    std::unique_ptr<DateTimeStm32> m_dateTimeStm32;
};

//====================
// SetTime()
//====================

TEST_F(FixtureDateTimeStm32, SetTime_ReturnsFalseIfConfigNotSet) {
    bool success = m_dateTimeStm32->SetTime(Time{});

    ASSERT_FALSE(success);
}

TEST_F(FixtureDateTimeStm32, SetTime_PerformsCorrectRegisterWrites) {
    m_dateTimeStm32->SetConfig(std::move(m_dateTimeConfigStm32));

    auto time = Time{
        .hours = 12,
        .minutes = 34,
        .seconds = 56,
        .milliseconds = 0,
    };

    const auto expected_CR = RTC_CR & ~(0b1 << 6);  // 24-hour format
    const auto expected_TR = 0 | 1 << 20            // tens of the hour
                             | 2 << 16              // singles of the hour
                             | 3 << 12              // tens of the minutes
                             | 4 << 8               // singles of the minutes
                             | 5 << 4               // tens of the seconds
                             | 6;                   // singles of the seconds

    const auto DR_before = RTC_DR;

    bool success = m_dateTimeStm32->SetTime(time);

    ASSERT_TRUE(success);
    ASSERT_EQ(RTC_CR, expected_CR);
    ASSERT_EQ(RTC_TR, expected_TR);
    ASSERT_EQ(RTC_DR, DR_before);
}

//====================
// GetTime()
//====================
TEST_F(FixtureDateTimeStm32, GetTime_ReturnsZerodTimeIfConfigNotSet) {
    auto receivedTime = m_dateTimeStm32->GetTime();

    ASSERT_EQ(receivedTime, Time({.hours = 0, .minutes = 0, .seconds = 0, .milliseconds = 0}));
}

TEST_F(FixtureDateTimeStm32, GetTime_ReturnsCorrectTime) {
    m_dateTimeStm32->SetConfig(std::move(m_dateTimeConfigStm32));

    auto time = Time{.hours = 12, .minutes = 34, .seconds = 56, .milliseconds = 0};

    m_dateTimeStm32->SetTime(time);

    auto receivedTime = m_dateTimeStm32->GetTime();

    ASSERT_EQ(receivedTime, time);
}

//====================
// SetDate()
//====================

TEST_F(FixtureDateTimeStm32, SetDate_ReturnsFalseIfConfigNotSet) {
    bool success = m_dateTimeStm32->SetDate(Date{});

    ASSERT_FALSE(success);
}

TEST_F(FixtureDateTimeStm32, SetDate_PerformsCorrectRegisterWrites) {
    m_dateTimeStm32->SetConfig(std::move(m_dateTimeConfigStm32));

    auto date = Date{
        .year = 2024,
        .month = 5,
        .day = 14,
        .weekday = Weekday::Saturday,
    };

    const auto expected_DR = 0 | 2 << 20  // tens of the year
                             | 4 << 16    // singles of the year
                             | 6 << 13    // week days unit
                             | 0 << 12    // tens of the month
                             | 5 << 8     // singles of the month
                             | 1 << 4     // tens of the day
                             | 4;         // singles of the day

    const auto TR_before = RTC_TR;

    bool success = m_dateTimeStm32->SetDate(date);

    ASSERT_TRUE(success);
    ASSERT_EQ(RTC_DR, expected_DR);
    ASSERT_EQ(RTC_TR, TR_before);
}

//====================
// GetDate()
//====================

TEST_F(FixtureDateTimeStm32, GetDate_ReturnsZerodDateIfConfigNotSet) {
    auto receivedDate = m_dateTimeStm32->GetDate();

    ASSERT_EQ(receivedDate, Date({.year = 0, .month = 0, .day = 0, .weekday = Weekday::Monday}));
}

TEST_F(FixtureDateTimeStm32, GetDate_ReturnsCorrectDate) {
    m_dateTimeStm32->SetConfig(std::move(m_dateTimeConfigStm32));

    auto date = Date{.year = 2024, .month = 5, .day = 12, .weekday = Weekday::Wednesday};

    m_dateTimeStm32->SetDate(date);

    auto receivedDate = m_dateTimeStm32->GetDate();

    ASSERT_EQ(receivedDate, date);
}

//====================
// SetDateTime()
//====================

TEST_F(FixtureDateTimeStm32, SetDateTime_ReturnsFalseIfConfigNotSet) {
    bool success = m_dateTimeStm32->SetDateTime(DateTime{});

    ASSERT_FALSE(success);
}

//====================
// GetDateTime()
//====================

TEST_F(FixtureDateTimeStm32, GetDateTime_ReturnsZerodDateTimeIfConfigNotSet) {
    auto receivedDateTime = m_dateTimeStm32->GetDateTime();

    ASSERT_EQ(receivedDateTime, DateTime({
                                    .time = Time({.hours = 0, .minutes = 0, .seconds = 0, .milliseconds = 0}),
                                    .date = Date({.year = 0, .month = 0, .day = 0, .weekday = Weekday::Monday}),
                                }));
}

TEST_F(FixtureDateTimeStm32, GetDateTime_ReturnsCorrectDateTime) {
    m_dateTimeStm32->SetConfig(std::move(m_dateTimeConfigStm32));

    auto dateTime = DateTime{
        .time = Time{.hours = 12, .minutes = 34, .seconds = 56, .milliseconds = 0},
        .date = Date{.year = 2024, .month = 5, .day = 12, .weekday = Weekday::Wednesday},
    };

    m_dateTimeStm32->SetDateTime(dateTime);

    auto receivedDateTime = m_dateTimeStm32->GetDateTime();

    ASSERT_EQ(receivedDateTime, dateTime);
}

//====================
// SetConfig()
//====================

TEST_F(FixtureDateTimeStm32, SetConfig_ReturnsFalseIfAnyPointerInConfigIfNullptr) {
    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        dateTimeConfigStm32.PWR_CR = nullptr;
        bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
        ASSERT_FALSE(success);
    }

    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        dateTimeConfigStm32.RTC_WPR = nullptr;
        bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
        ASSERT_FALSE(success);
    }

    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        dateTimeConfigStm32.RTC_ISR = nullptr;
        bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
        ASSERT_FALSE(success);
    }

    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        dateTimeConfigStm32.RTC_CR = nullptr;
        bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
        ASSERT_FALSE(success);
    }

    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        dateTimeConfigStm32.RTC_TR = nullptr;
        bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
        ASSERT_FALSE(success);
    }

    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        dateTimeConfigStm32.RTC_DR = nullptr;
        bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
        ASSERT_FALSE(success);
    }

    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        dateTimeConfigStm32.RTC_SSR = nullptr;
        bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
        ASSERT_FALSE(success);
    }
}

TEST_F(FixtureDateTimeStm32, SetConfig_ReturnsTrueIfAllPointersInConfigAreNotNullptr) {
    auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
    bool success = m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));

    ASSERT_TRUE(success);
}

//====================
// GetConfig()
//====================

TEST_F(FixtureDateTimeStm32, GetConfig_ReturnsNullptrIfSetConfigHasNotBeenCalled) {
    const auto& dateTimeConfigStm32 = m_dateTimeStm32->GetConfig();
    const auto expectedDateTimeConfigStm32 =
        DateTimeConfigStm32{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    ASSERT_EQ(dateTimeConfigStm32, expectedDateTimeConfigStm32);
}

TEST_F(FixtureDateTimeStm32, GetConfig_ReturnsConfigIfSetConfigHasBeenCalled) {
    {
        auto dateTimeConfigStm32 = m_dateTimeConfigStm32;
        m_dateTimeStm32->SetConfig(std::move(dateTimeConfigStm32));
    }

    const auto& dateTimeConfigStm32 = m_dateTimeStm32->GetConfig();

    ASSERT_EQ(dateTimeConfigStm32, m_dateTimeConfigStm32);
}

}  // namespace DateTimeStm32Testing
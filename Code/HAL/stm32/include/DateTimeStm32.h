#pragma once

#include "IDateTime.h"
#include "IDateTimeStm32.h"

class DateTimeStm32 : public IDateTime, IDateTimeStm32 {
   public:
    DateTimeStm32();
    ~DateTimeStm32() override = default;

    //=========================
    // ITime
    //=========================
    auto GetTime() -> Time override;
    auto SetTime(const Time& time) -> bool override;
    auto GetDate() -> Date override;
    auto SetDate(const Date& date) -> bool override;
    auto GetDateTime() -> DateTime override;
    auto SetDateTime(const DateTime& dateTime) -> bool override;

    //=========================
    // ITimeStm32
    //=========================
    auto SetConfig(const DateTimeConfigStm32&& dateTimeConfigStm32) -> bool override;
    auto GetConfig() -> const DateTimeConfigStm32& override;

   private:
    auto InternalSetTime(const Time& time) -> void;
    auto InternalSetDate(const Date& date) -> void;
    auto UnlockRtcRegisters() -> void;
    auto LockRtcRegisters() -> void;
    auto EnterInitializeMode() -> void;
    auto ExitInitializeMode() -> void;

    uint16_t m_yearBase;
    DateTimeConfigStm32 m_dateTimeConfigStm32;
};
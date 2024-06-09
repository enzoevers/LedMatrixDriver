#pragma once

#include "IDateTime.h"
#include "IDateTimeStm32.h"

namespace HAL::STM32 {
class DateTime : public HAL::IDateTime, HAL::STM32::IDateTime {
   public:
    DateTime();
    ~DateTime() override = default;

    //---------------
    // HAL::IDateTime
    //---------------
    auto GetTime() -> Types::Time override;
    auto SetTime(const Types::Time& time) -> bool override;
    auto GetDate() -> Types::Date override;
    auto SetDate(const Types::Date& date) -> bool override;
    auto GetDateTime() -> Types::DateTime override;
    auto SetDateTime(const Types::DateTime& dateTime) -> bool override;

    //---------------
    // HAL::STM32::IDateTime
    //---------------
    auto SetConfig(const DateTimeConfig&& dateTimeConfig) -> bool override;
    auto GetConfig() -> const DateTimeConfig& override;

   private:
    auto InternalSetTime(const Types::Time& time) -> void;
    auto InternalSetDate(const Types::Date& date) -> void;
    auto UnlockRtcRegisters() -> void;
    auto LockRtcRegisters() -> void;
    auto EnterInitializeMode() -> void;
    auto ExitInitializeMode() -> void;

    uint16_t m_yearBase;
    DateTimeConfig m_dateTimeConfig;
};

}  // namespace HAL::STM32
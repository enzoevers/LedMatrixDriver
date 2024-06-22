#include "States/SettingClock.h"

#include <cstring>

namespace Clock {

static constexpr auto invalidTime = Common::Types::Time{99, 99, 99, 0};
static constexpr auto zeroTime = Common::Types::Time{0, 0, 0, 0};

SettingClock::SettingClock(const Context&& context)
    : m_context(context), m_subState(States::None), m_startTimeForSubStateChange(invalidTime) {}

//---------------
// Clock::IState
//---------------

auto SettingClock::GetStateName() const -> const States { return States::SettingClock; }
auto SettingClock::HasSubState() const -> bool { return true; }
auto SettingClock::GetSubState() const -> const States { return m_subState; }

auto SettingClock::Enter() -> void {
    const auto screenResolution = m_context.display.GetResolution();
    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            m_context.display.SetPixel({x, y}, 0);
        }
    }
    m_context.display.UpdateDisplay();

    m_subState = States::SettingHour;
    m_timeToUpdate = m_context.dateTime.GetTime();
}

auto SettingClock::Update() -> void {
    // TODO: Clean this code up by moving the logic to separate functions
    const auto buttonAState = m_context.buttonA.GetState();
    const auto buttonBState = m_context.buttonB.GetState();
    const auto currentTime = m_context.dateTime.GetTime();

    UpdateSubState(buttonAState, buttonBState, currentTime);
    UpdateTime(buttonAState, buttonBState);
    ShowUpdatedTimeOnDisplay(currentTime);
}

auto SettingClock::Exit() -> void {
    m_context.dateTime.SetTime(m_timeToUpdate);

    const auto screenResolution = m_context.display.GetResolution();
    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            m_context.display.SetPixel({x, y}, 0);
        }
    }
    m_context.display.UpdateDisplay();
}

auto SettingClock::ShouldTransitionToState() -> States {
    if (m_subState == States::ShowingClock) {
        return States::ShowingClock;
    }

    return States::None;
}

//---------------
// Private
//---------------

auto SettingClock::UpdateSubState(const bool buttonAState, const bool buttonBState,
                                  const Common::Types::Time& currentTime) -> void {
    if (!(buttonAState && buttonBState)) {
        m_startTimeForSubStateChange = zeroTime;
        return;
    }

    if (m_startTimeForSubStateChange == zeroTime) {
        m_startTimeForSubStateChange = currentTime;
        return;
    }

    // Both buttons are pressed but they were not released before
    if (m_startTimeForSubStateChange == invalidTime) {
        return;
    }

    uint8_t secondsSinceStart = 0;
    if (currentTime.seconds > m_startTimeForSubStateChange.seconds) {
        secondsSinceStart = currentTime.seconds - m_startTimeForSubStateChange.seconds;
    } else if (currentTime.seconds < m_startTimeForSubStateChange.seconds) {
        secondsSinceStart = currentTime.seconds + (60 - m_startTimeForSubStateChange.seconds);
    } else {
        return;
    }

    if (secondsSinceStart < m_context.secondsForBtnPressTillSubStateChange) {
        return;
    }

    switch (m_subState) {
        case States::SettingHour:
            m_subState = States::SettingMinute;
            break;
        case States::SettingMinute:
            m_subState = States::SettingSecond;
            break;
        case States::SettingSecond:
            m_subState = States::ShowingClock;
            break;
        default:
            break;
    }
    m_startTimeForSubStateChange = invalidTime;
}

auto SettingClock::UpdateTime(const bool buttonAState, const bool buttonBState) -> void {
    if (!(buttonAState ^ buttonBState)) {
        // Both buttons are either pressed or not pressed
        return;
    }

    switch (m_subState) {
        case States::SettingHour: {
            if (buttonAState) {
                m_timeToUpdate.hours = (m_timeToUpdate.hours + 1) % 24;
            } else {
                // The same as minus one
                m_timeToUpdate.hours = (m_timeToUpdate.hours + 23) % 24;
            }

            break;
        }
        case States::SettingMinute: {
            if (buttonAState) {
                m_timeToUpdate.minutes = (m_timeToUpdate.minutes + 1) % 60;
            } else {
                // The same as minus one
                m_timeToUpdate.minutes = (m_timeToUpdate.minutes + 59) % 60;
            }

            break;
        }
        case States::SettingSecond: {
            if (buttonAState) {
                m_timeToUpdate.seconds = (m_timeToUpdate.seconds + 1) % 60;
            } else {
                // The same as minus one
                m_timeToUpdate.seconds = (m_timeToUpdate.seconds + 59) % 60;
            }

            break;
        }
        default:
            break;
    }
}

auto SettingClock::ShowUpdatedTimeOnDisplay(const Common::Types::Time& currentTime) const -> void {
    char timeTextBuffer[13];
    std::snprintf(&timeTextBuffer[0], sizeof(timeTextBuffer), "%.2u : %.2u : %.2u", m_timeToUpdate.hours,
                  m_timeToUpdate.minutes, m_timeToUpdate.seconds);

    if (currentTime.seconds % 2 == 0) {
        // Toggle the ':' characters every second
        timeTextBuffer[3] = ' ';
        timeTextBuffer[8] = ' ';
    }

    const auto pixelAreaSize = m_context.textProvider.GetRequiredSizeString(timeTextBuffer, 1);
    Monochrome pixelAreaData[pixelAreaSize.x * pixelAreaSize.y];
    std::memset(pixelAreaData, 0, sizeof(pixelAreaData));
    PixelArea<Monochrome> pixelArea{pixelAreaSize, pixelAreaData};
    Vec2D offset{0, 0};
    m_context.textProvider.SetString(timeTextBuffer, 1, offset, pixelArea);

    m_context.display.SetArea({5, 0}, pixelArea);
    m_context.display.UpdateDisplay();

    std::memset(pixelAreaData, 0, sizeof(pixelAreaData));
    m_context.display.SetArea({5, 0}, pixelArea);
}

}  // namespace Clock
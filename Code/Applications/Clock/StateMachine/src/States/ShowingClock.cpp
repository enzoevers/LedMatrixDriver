#include "States/ShowingClock.h"

#include <cstring>  // std::memset

namespace Clock {

static constexpr auto invalidTime = Common::Types::Time{99, 99, 99, 0};
static constexpr auto zeroTime = Common::Types::Time{0, 0, 0, 0};

ShowingClock::ShowingClock(const Context&& context) : m_context(context), m_startTimeForSetTimeCheck(invalidTime) {}

//---------------
// Clock::IState
//---------------
auto ShowingClock::GetStateName() const -> const States { return States::ShowingClock; }
auto ShowingClock::HasSubState() const -> bool { return false; }
auto ShowingClock::GetSubState() const -> const States { return States::None; }

auto ShowingClock::Enter() -> void {
    // Clear all screen pixels
    const auto screenResolution = m_context.display.GetResolution();
    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            m_context.display.SetPixel({x, y}, 0);
        }
    }
    m_context.display.UpdateDisplay();
}

auto ShowingClock::Update() -> void {
    auto currentDateTime = m_context.dateTime.GetDateTime();
    if (currentDateTime == m_lastDateTime) {
        return;
    }

    m_lastDateTime = currentDateTime;

    char timeTextBuffer[13];
    std::snprintf(&timeTextBuffer[0], sizeof(timeTextBuffer), "%.2u : %.2u : %.2u", currentDateTime.time.hours,
                  currentDateTime.time.minutes, currentDateTime.time.seconds);

    const auto pixelAreaSize = m_context.textProvider.GetRequiredSizeString(timeTextBuffer, 1);
    Monochrome pixelAreaData[pixelAreaSize.x * pixelAreaSize.y];
    std::memset(pixelAreaData, 0, sizeof(pixelAreaData));
    PixelArea<Monochrome> pixelArea{pixelAreaSize, pixelAreaData};
    Vec2D offset{0, 0};
    m_context.textProvider.SetString(timeTextBuffer, 1, offset, pixelArea);

    // TODO: xy position should be calculated to that the time is in the middle of the screen
    m_context.display.SetArea({5, 0}, pixelArea);
    m_context.display.UpdateDisplay();

    // Clear all screen pixels that where written
    std::memset(pixelAreaData, 0, sizeof(pixelAreaData));
    m_context.display.SetArea({5, 0}, pixelArea);
}

auto ShowingClock::Exit() -> void {
    // Clear all screen pixels
    const auto screenResolution = m_context.display.GetResolution();
    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            m_context.display.SetPixel({x, y}, 0);
        }
    }
    m_context.display.UpdateDisplay();
}

auto ShowingClock::ShouldTransitionToState() -> States {
    const auto buttonAState = m_context.buttonA.GetState();
    const auto buttonBState = m_context.buttonB.GetState();
    const auto currentTime = m_context.dateTime.GetTime();

    if (!(buttonAState && buttonBState)) {
        m_startTimeForSetTimeCheck = zeroTime;
        return States::None;
    }

    if (m_startTimeForSetTimeCheck == zeroTime) {
        m_startTimeForSetTimeCheck = currentTime;
        return States::None;
    }

    // Both buttons are pressed but they were not released before
    if (m_startTimeForSetTimeCheck == invalidTime) {
        return States::None;
    }

    uint8_t secondsSinceStart = 0;
    if (currentTime.seconds > m_startTimeForSetTimeCheck.seconds) {
        secondsSinceStart = currentTime.seconds - m_startTimeForSetTimeCheck.seconds;
    } else if (currentTime.seconds < m_startTimeForSetTimeCheck.seconds) {
        secondsSinceStart = currentTime.seconds + (60 - m_startTimeForSetTimeCheck.seconds);
    } else {
        return States::None;
    }

    if (secondsSinceStart < m_context.secondsForBtnPressTillChangeToSetTime) {
        return States::None;
    }

    return States::SettingClock;
}

}  // namespace Clock
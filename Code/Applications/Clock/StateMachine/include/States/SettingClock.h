#pragma once

#include "IState.h"

// Context
#include "IDateTime.h"
#include "IDisplay.h"
#include "IGPIOInput.h"
#include "ITextProvider.h"
#include "Monochrome.h"

namespace Clock {

class SettingClock : public IState {
   public:
    struct Context {
        IDisplay<Monochrome> &display;
        ITextProvider &textProvider;
        HAL::IDateTime &dateTime;
        HAL::IGPIOInput &buttonA;
        HAL::IGPIOInput &buttonB;
        /// @brief Expected to be less less than 60 second
        uint8_t secondsForBtnPressTillSubStateChange;
    };

    SettingClock(const Context &&context);
    ~SettingClock() override = default;

    //---------------
    // Clock::IState
    //---------------
    auto GetStateName() const -> const States override;
    auto HasSubState() const -> bool override;
    auto GetSubState() const -> const States override;
    auto Enter() -> void override;
    auto Update() -> void override;
    auto Exit() -> void override;
    auto ShouldTransitionToState() -> States override;

   private:
    auto UpdateSubState(const bool buttonAState, const bool buttonBState, const Common::Types::Time &currentTime)
        -> void;
    auto UpdateTime(const bool buttonAState, const bool buttonBState) -> void;
    auto ShowUpdatedTimeOnDisplay(const Common::Types::Time &currentTime) const -> void;

    const Context m_context;
    States m_subState;
    Common::Types::Time m_timeToUpdate;

    Common::Types::Time m_startTimeForSubStateChange;
};

}  // namespace Clock
#pragma once

#include "IState.h"
//----------
#include "Types/DateTimeData.h"
#include "VecXD.h"

// Context
#include "IDateTime.h"
#include "IDisplay.h"
#include "IGPIOInput.h"
#include "ITextProvider.h"
#include "Monochrome.h"

namespace Clock {

class ShowingClock : public IState {
   public:
    struct Context {
        IDisplay<Monochrome> &display;
        ITextProvider &textProvider;
        HAL::IDateTime &dateTime;
        HAL::IGPIOInput &buttonA;
        HAL::IGPIOInput &buttonB;
        uint8_t secondsForBtnPressTillChangeToSetTime;
    };

    ShowingClock(const Context &&context);
    ~ShowingClock() override = default;

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
    const Context m_context;
    Common::Types::DateTime m_lastDateTime;

    Common::Types::Time m_startTimeForSetTimeCheck;
};

}  // namespace Clock
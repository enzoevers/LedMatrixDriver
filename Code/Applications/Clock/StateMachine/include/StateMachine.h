#pragma once

#include "IStateMachine.h"

// Displays
#include "IDisplay.h"
#include "ITextProvider.h"
#include "Monochrome.h"

// Time
#include "IDateTime.h"

// Delay
#include "IDelay.h"

// Control input
#include "IGPIOInput.h"

//-----

#include <memory>

namespace Clock {

/*!
 @brief Interface to define the main state machine that handles the internal state transitions.
*/
class StateMachine : public IStateMachine {
   public:
    StateMachine(IDisplay<Monochrome>& display, ITextProvider& textProvider, HAL::IDateTime& dateTime,
                 HAL::IDelay& delay, HAL::IGPIOInput& buttonA, HAL::IGPIOInput& buttonB);
    ~StateMachine() override;

    //---------------
    // Clock::IStateMachine
    //---------------
    auto Start() -> void override;
    auto Update() -> void override;
    auto GetCurrentState() const -> const IState* override;

   private:
    auto TransitionToState(const States newState) -> void;
    auto HandleStateTransition(std::unique_ptr<IState> newState) -> void;

    IDisplay<Monochrome>& m_display;
    ITextProvider& m_textProvider;
    HAL::IDateTime& m_dateTime;
    HAL::IDelay& m_delay;
    HAL::IGPIOInput& m_buttonA;
    HAL::IGPIOInput& m_buttonB;

    std::unique_ptr<IState> m_currentState;
};

}  // namespace Clock
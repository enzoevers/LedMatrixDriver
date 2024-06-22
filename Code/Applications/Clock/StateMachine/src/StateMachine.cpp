#include "StateMachine.h"

// States
#include "States/SettingClock.h"
#include "States/ShowingClock.h"

namespace Clock {

StateMachine::StateMachine(IDisplay<Monochrome>& display, ITextProvider& textProvider, HAL::IDateTime& dateTime,
                           HAL::IDelay& delay, HAL::IGPIOInput& buttonA, HAL::IGPIOInput& buttonB)
    : m_display(display),
      m_textProvider(textProvider),
      m_dateTime(dateTime),
      m_delay(delay),
      m_buttonA(buttonA),
      m_buttonB(buttonB) {}

StateMachine::~StateMachine() {
    if (m_currentState) {
        m_currentState->Exit();
    }
}

//---------------
// Clock::IStateMachine
//---------------

auto StateMachine::Start() -> void { TransitionToState(States::ShowingClock); }

auto StateMachine::Update() -> void {
    if (!m_currentState) {
        return;
    }

    const auto nextState = m_currentState->ShouldTransitionToState();
    if (nextState != States::None) {
        TransitionToState(nextState);
        return;
    }

    m_currentState->Update();
}

auto StateMachine::GetCurrentState() const -> const IState* {
    if (!m_currentState) {
        return nullptr;
    }

    return m_currentState.get();
}

//---------------
// Private
//---------------
auto StateMachine::TransitionToState(const States newState) -> void {
    switch (newState) {
        case States::ShowingClock: {
            auto showingClockState =
                std::make_unique<ShowingClock>(ShowingClock::Context{.display = m_display,
                                                                     .textProvider = m_textProvider,
                                                                     .dateTime = m_dateTime,
                                                                     .buttonA = m_buttonA,
                                                                     .buttonB = m_buttonB,
                                                                     .secondsForBtnPressTillChangeToSetTime = 3});
            HandleStateTransition(std::move(showingClockState));
            break;
        }
        case States::SettingClock: {
            auto settingClockState =
                std::make_unique<SettingClock>(SettingClock::Context{.display = m_display,
                                                                     .textProvider = m_textProvider,
                                                                     .dateTime = m_dateTime,
                                                                     .buttonA = m_buttonA,
                                                                     .buttonB = m_buttonB,
                                                                     .secondsForBtnPressTillSubStateChange = 1});
            HandleStateTransition(std::move(settingClockState));
            break;
        }
        default: {
            // Other states are not substates of one of the states mentioned above.
            break;
        }
    }
}

auto StateMachine::HandleStateTransition(std::unique_ptr<IState> newState) -> void {
    if (m_currentState != nullptr) {
        m_currentState->Exit();
    }
    m_currentState = std::move(newState);
    m_currentState->Enter();
}

};  // namespace Clock
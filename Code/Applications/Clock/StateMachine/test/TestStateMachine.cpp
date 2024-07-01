#include "StateMachine.h"
//---------------
#include "MockIDateTime.h"
#include "MockIDelay.h"
#include "MockIDisplay.h"
#include "MockIGPIOInput.h"
#include "MockITextProvider.h"
//---------------
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace Clock {

namespace StateMachineTesting {

class FixtureStateMachine : public Test {
   public:
    FixtureStateMachine()
        : m_mockIDisplayMonochrome(std::make_unique<MockIDisplayMonochrome>()),
          m_mockITextProvider(std::make_unique<MockITextProvider>()),
          m_mockIDateTime(std::make_unique<HAL::Mock::MockIDateTime>()),
          m_mockIDelay(std::make_unique<HAL::Mock::MockIDelay>()),
          m_mockIGPIOInputA(std::make_unique<HAL::Mock::MockIGPIOInput>()),
          m_mockIGPIOInputB(std::make_unique<HAL::Mock::MockIGPIOInput>()),
          m_stateMachine(std::make_unique<StateMachine>(*m_mockIDisplayMonochrome, *m_mockITextProvider,
                                                        *m_mockIDateTime, *m_mockIDelay, *m_mockIGPIOInputA,
                                                        *m_mockIGPIOInputB)) {}

    ~FixtureStateMachine() {}

    std::unique_ptr<MockIDisplayMonochrome> m_mockIDisplayMonochrome;
    std::unique_ptr<MockITextProvider> m_mockITextProvider;
    std::unique_ptr<HAL::Mock::MockIDateTime> m_mockIDateTime;
    std::unique_ptr<HAL::Mock::MockIDelay> m_mockIDelay;
    std::unique_ptr<HAL::Mock::MockIGPIOInput> m_mockIGPIOInputA;
    std::unique_ptr<HAL::Mock::MockIGPIOInput> m_mockIGPIOInputB;

    std::unique_ptr<StateMachine> m_stateMachine;
};

//====================
// Constructor
//====================

//---------------
// IstateMachine
//---------------

//====================
// Start()
//====================

TEST_F(FixtureStateMachine, Start_SetsCorrectInitialState) {
    m_stateMachine->Start();

    auto pCurrentState = m_stateMachine->GetCurrentState();
    ASSERT_NE(pCurrentState, nullptr);
    ASSERT_EQ(pCurrentState->GetStateName(), States::ShowingClock);
}

//====================
// GetCurrentState()
//====================

TEST_F(FixtureStateMachine, GetCurrentState_ReturnsNullptrWhenStartHasNotBeenCalledYet) {
    auto pCurrentState = m_stateMachine->GetCurrentState();
    ASSERT_EQ(pCurrentState, nullptr);
}

}  // namespace StateMachineTesting
}  // namespace Clock
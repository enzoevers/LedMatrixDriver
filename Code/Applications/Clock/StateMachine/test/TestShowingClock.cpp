#include "States/ShowingClock.h"
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

namespace ShowingClockTesting {

class FixtureShowingClock : public Test {
   public:
    FixtureShowingClock()
        : m_mockIDisplayMonochrome(std::make_unique<NiceMock<MockIDisplayMonochrome>>()),
          m_mockITextProvider(std::make_unique<NiceMock<MockITextProvider>>()),
          m_mockIDateTime(std::make_unique<NiceMock<HAL::Mock::MockIDateTime>>()),
          m_mockIDelay(std::make_unique<NiceMock<HAL::Mock::MockIDelay>>()),
          m_mockIGPIOInputA(std::make_unique<NiceMock<HAL::Mock::MockIGPIOInput>>()),
          m_mockIGPIOInputB(std::make_unique<NiceMock<HAL::Mock::MockIGPIOInput>>()),
          m_secondsForBtnPressTillChangeToSetTime(2),
          m_showingClock(std::make_unique<ShowingClock>(ShowingClock::Context{
              .display = *m_mockIDisplayMonochrome,
              .textProvider = *m_mockITextProvider,
              .dateTime = *m_mockIDateTime,
              .buttonA = *m_mockIGPIOInputA,
              .buttonB = *m_mockIGPIOInputB,
              .secondsForBtnPressTillChangeToSetTime = m_secondsForBtnPressTillChangeToSetTime})) {}

    ~FixtureShowingClock() {}

    std::unique_ptr<NiceMock<MockIDisplayMonochrome>> m_mockIDisplayMonochrome;
    std::unique_ptr<NiceMock<MockITextProvider>> m_mockITextProvider;
    std::unique_ptr<NiceMock<HAL::Mock::MockIDateTime>> m_mockIDateTime;
    std::unique_ptr<NiceMock<HAL::Mock::MockIDelay>> m_mockIDelay;
    std::unique_ptr<NiceMock<HAL::Mock::MockIGPIOInput>> m_mockIGPIOInputA;
    std::unique_ptr<NiceMock<HAL::Mock::MockIGPIOInput>> m_mockIGPIOInputB;
    uint8_t m_secondsForBtnPressTillChangeToSetTime;

    std::unique_ptr<ShowingClock> m_showingClock;
};

//====================
// Constructor
//====================

//---------------
// Istate
//---------------

//====================
// GetStateName()
//====================

TEST_F(FixtureShowingClock, GetStateName_ReturnsShowingClock) {
    const auto stateName = m_showingClock->GetStateName();
    ASSERT_EQ(stateName, States::ShowingClock);
}

//====================
// Enter()
//====================

TEST_F(FixtureShowingClock, Enter_ClearsAllScreenPixels) {
    const auto screenResolution = Vec2D{.x = 25, .y = 30};

    InSequence s;

    EXPECT_CALL(*m_mockIDisplayMonochrome, GetResolution()).WillOnce(Return(screenResolution));

    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            EXPECT_CALL(*m_mockIDisplayMonochrome, SetPixel(Vec2D{x, y}, 0)).Times(1);
        }
    }

    EXPECT_CALL(*m_mockIDisplayMonochrome, UpdateDisplay()).Times(1);

    m_showingClock->Enter();
}

//====================
// Update()
//====================

TEST_F(FixtureShowingClock, Update_GetsTheTimeAndDateOnce) {
    EXPECT_CALL(*m_mockIDateTime, GetDateTime()).Times(1);
    m_showingClock->Update();
}

TEST_F(FixtureShowingClock, Update_DoesNotUpdateDisplayIfTimeHasNotChanged) {
    InSequence s;

    const auto dateTime = Common::Types::DateTime({
        .time = {.hours = 14, .minutes = 4, .seconds = 25, .milliseconds = 0},
        .date = {.year = 2021, .month = 9, .day = 1, .weekday = Common::Types::Weekday::Wednesday},
    });

    // First call

    EXPECT_CALL(*m_mockIDateTime, GetDateTime()).WillOnce(Return(dateTime));
    m_showingClock->Update();

    // Second call

    EXPECT_CALL(*m_mockIDateTime, GetDateTime()).WillOnce(Return(dateTime));
    // The thing to test
    EXPECT_CALL(*m_mockIDisplayMonochrome, UpdateDisplay()).Times(0);
    m_showingClock->Update();
}

TEST_F(FixtureShowingClock, Update_UpdatesDisplayIfTimeHasChanged) {
    InSequence s;

    auto dateTime = Common::Types::DateTime({
        .time = {.hours = 14, .minutes = 4, .seconds = 25, .milliseconds = 0},
        .date = {.year = 2021, .month = 9, .day = 1, .weekday = Common::Types::Weekday::Wednesday},
    });

    // First call

    EXPECT_CALL(*m_mockIDateTime, GetDateTime()).WillOnce(Return(dateTime));
    m_showingClock->Update();

    // Second call

    dateTime.time.seconds += 1;
    EXPECT_CALL(*m_mockIDateTime, GetDateTime()).WillOnce(Return(dateTime));
    // The thing to test
    EXPECT_CALL(*m_mockIDisplayMonochrome, UpdateDisplay()).Times(1);
    m_showingClock->Update();
}

//====================
// Exit()
//====================

TEST_F(FixtureShowingClock, Exit_ClearsAllScreenPixels) {
    const auto screenResolution = Vec2D{.x = 25, .y = 30};

    InSequence s;

    EXPECT_CALL(*m_mockIDisplayMonochrome, GetResolution()).WillOnce(Return(screenResolution));

    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            EXPECT_CALL(*m_mockIDisplayMonochrome, SetPixel(Vec2D{x, y}, 0)).Times(1);
        }
    }

    EXPECT_CALL(*m_mockIDisplayMonochrome, UpdateDisplay()).Times(1);

    m_showingClock->Exit();
}

//====================
// ShouldTransitionToState()
//====================

TEST_F(FixtureShowingClock, ShouldTransitionToState_ReturnsStateNoneIfButtonsAreNotPressedLongEnough) {
    const auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);
}

TEST_F(FixtureShowingClock, ShouldTransitionToState_DoesNotTransitionIfButtonsAreNotReleasedToStartWith) {
    InSequence s;

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);
}

TEST_F(FixtureShowingClock,
       ShouldTransitionToState_ReturnsStateSettingClockIfButtonsAresPressedExactlyLongEnoughOnSecondCall) {
    InSequence s;

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    // The buttons should be pressed two second
    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::SettingClock);
}

TEST_F(FixtureShowingClock,
       ShouldTransitionToState_ReturnsStateSettingClockIfButtonsAresPressedLongerThanRequiredOnSecondCall) {
    InSequence s;

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime + 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::SettingClock);
}

TEST_F(FixtureShowingClock,
       ShouldTransitionToState_ReturnsStateSettingClockIfButtonsAresPressedExactlyLongEnoughOnThirdCall) {
    InSequence s;

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    // This assumes m_secondsForBtnPressTillChangeToSetTime >= 2
    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime - 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::SettingClock);
}

TEST_F(
    FixtureShowingClock,
    ShouldTransitionToState_ReturnsStateSettingClockIfButtonsAresPressedLongEnoughOnSecondCallAndTimeValuesOverflowed) {
    InSequence s;

    auto receivedTime = Common::Types::Time{.hours = 23, .minutes = 59, .seconds = 58};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    // The buttons should be pressed two seconds
    receivedTime =
        Common::Types::Time{.hours = 0,
                            .minutes = 0,
                            .seconds = uint8_t((uint8_t(59) + m_secondsForBtnPressTillChangeToSetTime) % uint8_t(60))};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::SettingClock);
}

TEST_F(FixtureShowingClock, ShouldTransitionToState_ReturnsStateNoneIfButtonsAreReleasedBeforeRequiredTime) {
    InSequence s;

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    // This assumes m_secondsForBtnPressTillChangeToSetTime >= 2
    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime - 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::SettingClock);
}

TEST_F(FixtureShowingClock, ShouldTransitionToState_ReturnsStateNoneIfOneButtonIsReleasedBeforeRequiredTime) {
    InSequence s;

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    auto newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    // This assumes m_secondsForBtnPressTillChangeToSetTime >= 2
    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime - 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    // +1 to get to m_secondsForBtnPressTillChangeToSetTime
    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);

    receivedTime.seconds += m_secondsForBtnPressTillChangeToSetTime;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    newState = m_showingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::SettingClock);
}

//====================
// HasSubState()
//====================

TEST_F(FixtureShowingClock, HasSubState_ReturnsFalse) {
    const auto hasSubState = m_showingClock->HasSubState();
    ASSERT_FALSE(hasSubState);
}

//====================
// GetSubState()
//====================

TEST_F(FixtureShowingClock, GetSubState_ReturnsNone) {
    const auto subState = m_showingClock->GetSubState();
    ASSERT_EQ(subState, States::None);
}

}  // namespace ShowingClockTesting
}  // namespace Clock
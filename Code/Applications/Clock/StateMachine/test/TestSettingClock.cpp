#include "States/SettingClock.h"
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

namespace SettingClockTesting {

class FixtureSettingClock : public Test {
   public:
    FixtureSettingClock()
        : m_mockIDisplayMonochrome(std::make_unique<NiceMock<MockIDisplayMonochrome>>()),
          m_mockITextProvider(std::make_unique<NiceMock<MockITextProvider>>()),
          m_mockIDateTime(std::make_unique<NiceMock<HAL::Mock::MockIDateTime>>()),
          m_mockIDelay(std::make_unique<NiceMock<HAL::Mock::MockIDelay>>()),
          m_mockIGPIOInputA(std::make_unique<NiceMock<HAL::Mock::MockIGPIOInput>>()),
          m_mockIGPIOInputB(std::make_unique<NiceMock<HAL::Mock::MockIGPIOInput>>()),
          m_secondsForBtnPressTillSubStateChange(2),
          m_settingClock(std::make_unique<SettingClock>(
              SettingClock::Context{.display = *m_mockIDisplayMonochrome,
                                    .textProvider = *m_mockITextProvider,
                                    .dateTime = *m_mockIDateTime,
                                    .buttonA = *m_mockIGPIOInputA,
                                    .buttonB = *m_mockIGPIOInputB,
                                    .secondsForBtnPressTillSubStateChange = m_secondsForBtnPressTillSubStateChange})) {}

    ~FixtureSettingClock() {}

    std::unique_ptr<NiceMock<MockIDisplayMonochrome>> m_mockIDisplayMonochrome;
    std::unique_ptr<NiceMock<MockITextProvider>> m_mockITextProvider;
    std::unique_ptr<NiceMock<HAL::Mock::MockIDateTime>> m_mockIDateTime;
    std::unique_ptr<NiceMock<HAL::Mock::MockIDelay>> m_mockIDelay;
    std::unique_ptr<NiceMock<HAL::Mock::MockIGPIOInput>> m_mockIGPIOInputA;
    std::unique_ptr<NiceMock<HAL::Mock::MockIGPIOInput>> m_mockIGPIOInputB;
    uint8_t m_secondsForBtnPressTillSubStateChange;

    std::unique_ptr<SettingClock> m_settingClock;
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

TEST_F(FixtureSettingClock, GetStateName_ReturnsShowingClock) {
    const auto stateName = m_settingClock->GetStateName();
    ASSERT_EQ(stateName, States::SettingClock);
}

//====================
// Enter()
//====================

TEST_F(FixtureSettingClock, Enter_GetSubStateReturnsSettingHoursIfEnterIsCalled) {
    m_settingClock->Enter();

    const auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);
}

TEST_F(FixtureSettingClock, Enter_ClearsAllScreenPixels) {
    const auto screenResolution = Vec2D{.x = 25, .y = 30};

    InSequence s;

    EXPECT_CALL(*m_mockIDisplayMonochrome, GetResolution()).WillOnce(Return(screenResolution));

    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            EXPECT_CALL(*m_mockIDisplayMonochrome, SetPixel(Vec2D{x, y}, 0)).Times(1);
        }
    }

    EXPECT_CALL(*m_mockIDisplayMonochrome, UpdateDisplay()).Times(1);

    m_settingClock->Enter();
}

//====================
// Update()
//====================

TEST_F(
    FixtureSettingClock,
    Update_DoesNotGoToSettingMinuteIfSubStateIsSettingHourAndButtonsArePressedExactlyTheNeededTimeWithoutTheButtonsBeingReleasedBefore) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);
}

TEST_F(FixtureSettingClock, Update_GoesToSettingMinuteIfSubStateIsSettingHourAndButtonsArePressedExactlyTheNeededTime) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);
}

TEST_F(FixtureSettingClock, Update_GoesToSettingMinuteIfSubStateIsSettingHourAndButtonsArePressedLongerThanNeededTime) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange + 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);
}

TEST_F(FixtureSettingClock,
       Update_GoesToSettingMinuteIfSubStateIsSettingHourAndButtonsArePressedTwiceAsLongAsNeededTime) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += 2 * m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);
}

TEST_F(FixtureSettingClock, Update_StaysInSettingMinuteIfButtonsAreNotReleased) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);

    // Buttons are not released before this
    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);
}

TEST_F(FixtureSettingClock, Update_StaysInSettingMinuteIfOnlyOneButtonIsReleased) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);

    //  This assumes that
    //  `m_secondsForBtnPressTillSubStateChange > 1`
    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange - 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);

    // Only one button is released before this
    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);
}

TEST_F(FixtureSettingClock,
       Update_GoesToSettingMinuteIfSubStateIsSettingHourAndButtonsArePressedExactlyTheExitPressTimeWithOverflownTime) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 23, .minutes = 59, .seconds = 58};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime =
        Common::Types::Time{.hours = 0, .minutes = 0, .seconds = uint8_t(m_secondsForBtnPressTillSubStateChange - 1)};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);
}

TEST_F(FixtureSettingClock,
       Update_DoesNotGoToSettingMinuteIfSubStateIsSettingHourAndButtonsAreReleasedBeforeNeededPressTime) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    // This assumes that
    // `m_secondsForBtnPressTillSubStateChange > 1`
    receivedTime.seconds += uint8_t(m_secondsForBtnPressTillSubStateChange - 1);
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingHour);

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);
}

TEST_F(
    FixtureSettingClock,
    Update_GoesToSettingSecondIfSubStateIsSettingMinuteAndButtonsArePressedExactlyTheNeededTimeAfterReleasingAndRepressingButtons) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    {
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        //  Going to States::SettingMinute
        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        auto subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingMinute);
    }

    // Releasing the buttons before re-pressing
    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);

    // Re-pressing the buttons
    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange - 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingMinute);

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::SettingSecond);
}

TEST_F(FixtureSettingClock,
       Update_GoesToShowingClockIfSubStateIsSettingSecondAndButtonsArePressedExactlyTheNeededTime) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    {
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        //  Going to States::SettingMinute
        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        auto subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingMinute);

        //  Going to States::SettingSecond
        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingSecond);
    }

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::ShowingClock);
}

TEST_F(FixtureSettingClock,
       Update_StaysInShowingClockIfSubStateIsShowingClockAndButtonsArePressedExactlyTheNeededTime) {
    InSequence s;

    m_settingClock->Enter();

    auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
    {
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        //  Going to States::SettingMinute
        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        auto subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingMinute);

        //  Going to States::SettingSecond
        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingSecond);

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::ShowingClock);
    }

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += 1;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();

    receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
    EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
    m_settingClock->Update();
    auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::ShowingClock);
}

//====================
// Exit()
//====================

TEST_F(FixtureSettingClock, Exit_ClearsAllScreenPixels) {
    const auto screenResolution = Vec2D{.x = 25, .y = 30};

    InSequence s;

    EXPECT_CALL(*m_mockIDisplayMonochrome, GetResolution()).WillOnce(Return(screenResolution));

    for (uint32_t y = 0; y < screenResolution.y; ++y) {
        for (uint32_t x = 0; x < screenResolution.x; ++x) {
            EXPECT_CALL(*m_mockIDisplayMonochrome, SetPixel(Vec2D{x, y}, 0)).Times(1);
        }
    }

    EXPECT_CALL(*m_mockIDisplayMonochrome, UpdateDisplay()).Times(1);

    m_settingClock->Exit();
}

TEST_F(FixtureSettingClock, Exit_UpdatesTheTime) {
    EXPECT_CALL(*m_mockIDateTime, SetTime(_)).Times(1);

    m_settingClock->Exit();
}

// TODO: Test that the correct time is set

//====================
// ShouldTransitionToState()
//====================

TEST_F(FixtureSettingClock, ShouldTransitionToState_ReturnsNoneIfCurrentSubStateIsNone) {
    // Note that Enter() is not called here
    const auto newState = m_settingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);
}

TEST_F(FixtureSettingClock, ShouldTransitionToState_ReturnsNoneIfCurrentSubStateIsSettingHour) {
    m_settingClock->Enter();

    const auto newState = m_settingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);
}

TEST_F(FixtureSettingClock, ShouldTransitionToState_ReturnsNoneIfCurrentSubStateIsSettingMinute) {
    {
        InSequence s;

        m_settingClock->Enter();

        auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        auto subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingMinute);
    }

    auto newState = m_settingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);
}

TEST_F(FixtureSettingClock, ShouldTransitionToState_ReturnsNoneIfCurrentSubStateIsSettingSecond) {
    {
        InSequence s;

        m_settingClock->Enter();

        auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        auto subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingMinute);

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingSecond);
    }

    auto newState = m_settingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::None);
}

TEST_F(FixtureSettingClock, ShouldTransitionToState_ReturnsShowingClockIfCurrentSubStateIsShowingClock) {
    {
        InSequence s;

        m_settingClock->Enter();

        auto receivedTime = Common::Types::Time{.hours = 3, .minutes = 12, .seconds = 20};
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        auto subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingMinute);

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::SettingSecond);

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(false));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += 1;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();

        receivedTime.seconds += m_secondsForBtnPressTillSubStateChange;
        EXPECT_CALL(*m_mockIGPIOInputA, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIGPIOInputB, GetState()).WillOnce(Return(true));
        EXPECT_CALL(*m_mockIDateTime, GetTime()).WillOnce(Return(receivedTime));
        m_settingClock->Update();
        subState = m_settingClock->GetSubState();
        ASSERT_EQ(subState, States::ShowingClock);
    }

    auto newState = m_settingClock->ShouldTransitionToState();
    ASSERT_EQ(newState, States::ShowingClock);
}

//====================
// HasSubState()
//====================

TEST_F(FixtureSettingClock, HasSubState_ReturnsTrue) {
    const auto hasSubState = m_settingClock->HasSubState();
    ASSERT_TRUE(hasSubState);
}

//====================
// GetSubState()
//====================

TEST_F(FixtureSettingClock, GetSubState_ReturnsNoneIfEnterIfNotCalledYet) {
    const auto subState = m_settingClock->GetSubState();
    ASSERT_EQ(subState, States::None);
}

}  // namespace SettingClockTesting
}  // namespace Clock
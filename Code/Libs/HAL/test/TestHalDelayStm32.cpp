#include "DelayStm32.h"
//-----
#include <thread>

#include "fuzztest/fuzztest.h"
#include "fuzztest/googletest_fixture_adapter.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace HAL::STM32 {

namespace DelayStm32Testing {

class FixtureDelayStm32 : public Test {
   public:
    FixtureDelayStm32()
        : m_statusRegister(0xFFFFFFFF),
          m_autoReloadRegister(0),
          m_controlRegister(0),
          m_prescalerRegister(0),
          m_timerInputFrequencyInHertz(10000000),
          m_updateInterruptMask(0x1),
          m_counterEnableMask(0x1 << 1),
          m_delayConfig({&m_statusRegister, &m_autoReloadRegister, &m_controlRegister, &m_prescalerRegister,
                         m_timerInputFrequencyInHertz, m_updateInterruptMask, m_counterEnableMask}),
          m_defaultMicrosecondsDelay(1000),
          m_microsecondsWaitAfterJoinable(100000) {}

    auto CreateIDelayStm32() -> std::unique_ptr<IDelay> { return std::make_unique<Delay>(); }
    auto CreateDelayStm32() -> std::unique_ptr<Delay> { return std::make_unique<Delay>(); }
    auto CreateDelayStm32Configured() -> std::unique_ptr<Delay> {
        auto delayStm32 = CreateDelayStm32();
        delayStm32->SetupConfiguration(std::move(m_delayConfig));
        return delayStm32;
    }

    volatile uint32_t m_statusRegister;
    volatile uint32_t m_autoReloadRegister;
    volatile uint32_t m_controlRegister;
    volatile uint32_t m_prescalerRegister;
    uint32_t m_timerInputFrequencyInHertz;
    uint32_t m_updateInterruptMask;
    uint32_t m_counterEnableMask;

    DelayConfig m_delayConfig;

    uint32_t m_defaultMicrosecondsDelay;
    uint32_t m_microsecondsWaitAfterJoinable;
};

class FixtureDelayStm32FuzzTests : public PerFuzzTestFixtureAdapter<FixtureDelayStm32> {
   public:
    //--------------------
    // SetupConfiguration()
    //--------------------

    void SetupConfiguration_ReturnsFalseIfUpdateInterruptMaskDoesNotHaveOneBitSet(uint32_t mask) {
        if ((mask != 0) && (((mask & (mask - 1)) == 0))) {
            // By setting two bits, there will always be more than one bit set.
            // Even if mask is 0x10 or 0x01
            mask |= 0x11;
        }

        auto iDelayStm32 = CreateIDelayStm32();

        m_delayConfig.updateInterruptMask = mask;

        EXPECT_FALSE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
    }

    void SetupConfiguration_ReturnsTrueIfUpdateInterruptMaskHasOneBitSet(uint8_t shifts) {
        auto iDelayStm32 = CreateIDelayStm32();

        uint32_t mask = 0x1 << shifts;

        m_delayConfig.updateInterruptMask = mask;

        EXPECT_TRUE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
    }

    void SetupConfiguration_ReturnsFalseIfCounterEnableMaskDoesNotHaveOneBitSet(uint32_t mask) {
        if ((mask != 0) && (((mask & (mask - 1)) == 0))) {
            // By setting two bits, there will always be more than one bit set.
            // Even if mask is 0x10 or 0x01
            mask |= 0x11;
        }

        auto iDelayStm32 = CreateIDelayStm32();

        m_delayConfig.counterEnableMask = mask;

        EXPECT_FALSE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
    }

    void SetupConfiguration_ReturnsTrueIfCounterEnableMaskHasOneBitSet(uint8_t shifts) {
        auto iDelayStm32 = CreateIDelayStm32();

        uint32_t mask = 0x1 << shifts;

        m_delayConfig.counterEnableMask = mask;

        EXPECT_TRUE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
    }
};

//---------------
// IDelay
//---------------

//====================
// SynchronousWait_us()
//
// Note: If function doesn't behave correctly, SynchronousWait_us() might become an infinite loop.
//       Maybe some extra thread management could safely terminate the test after a certain amount of time.
//====================

TEST_F(FixtureDelayStm32, SynchronousWait_us_ReturnsFalseIfWaitAmountIsTooShortBasedOnClockHertz) {
    auto delayStm32 = CreateDelayStm32();

    m_delayConfig.timerInputFrequencyInHertz = 1000;
    delayStm32->SetupConfiguration(std::move(m_delayConfig));

    auto result = false;

    const auto microsecondsWait = 999;
    auto delayThread = std::jthread([&]() { result = delayStm32->SynchronousWait_us(microsecondsWait); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    ASSERT_FALSE(result);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

TEST_F(FixtureDelayStm32, SynchronousWait_us_ReturnsFalseIfWaitAmountIsTooLongBasedOnClockHertz) {
    auto delayStm32 = CreateDelayStm32();

    m_delayConfig.timerInputFrequencyInHertz = UINT32_MAX;

    const auto maxPrescaler = static_cast<uint32_t>(UINT16_MAX) + 1;
    const auto clockHertzWithHighestPrescaler = m_delayConfig.timerInputFrequencyInHertz / (maxPrescaler + 1);

    // clockHertzWithHighestPrescaler is now 65,535.99998 which becomes 65,535

    const auto clocksPerMicrosecond = clockHertzWithHighestPrescaler / 1000000.0f;  // 0,065535

    const auto maxTimerClocks = static_cast<uint32_t>(UINT16_MAX);

    // (2^16 - 1) / 0,065535 = 65535 / 0,065535 = 1000000
    const auto maxMicroSeconds = static_cast<uint32_t>(maxTimerClocks / clocksPerMicrosecond);

    delayStm32->SetupConfiguration(std::move(m_delayConfig));

    auto result = false;

    const auto microsecondsWait = maxMicroSeconds + 1;
    auto delayThread = std::jthread([&]() { result = delayStm32->SynchronousWait_us(microsecondsWait); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    ASSERT_FALSE(result);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

TEST_F(FixtureDelayStm32,
       DISABLED_SynchronousWait_us_SetsPrescalerToZeroAndCorrectArrIfNoPrescalerNeededBasedOnClockHertz) {
    auto delayStm32 = CreateDelayStm32();

    m_delayConfig.timerInputFrequencyInHertz = 1000000;  // 1 MHz => 1us per clock
    delayStm32->SetupConfiguration(std::move(m_delayConfig));

    const auto microsecondsWait = static_cast<uint32_t>(UINT16_MAX);
    auto delayThread = std::jthread([&]() { delayStm32->SynchronousWait_us(microsecondsWait); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    ASSERT_EQ(m_prescalerRegister, 0);
    ASSERT_EQ(m_autoReloadRegister, UINT16_MAX);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

// TODO: Fix
TEST_F(FixtureDelayStm32,
       SynchronousWait_us_SetsPrescalerToOneAndCorrectArrIfNeededBasedOnClockHertzIfRequiredDelayEven) {
    auto delayStm32 = CreateDelayStm32();

    const uint32_t inputHertz = 1000000;  // 1 MHz => 1us per clock
    m_delayConfig.timerInputFrequencyInHertz = inputHertz;
    delayStm32->SetupConfiguration(std::move(m_delayConfig));

    // The ARR register is assumed ot be 16-bits. At a clock of 1 MHz this means that
    // at 2^16 microseconds the prescaler should be at least 1.
    const auto microsecondsWait = static_cast<uint32_t>(UINT16_MAX) + 1;
    auto delayThread = std::jthread([&]() { delayStm32->SynchronousWait_us(microsecondsWait); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    ASSERT_EQ(m_prescalerRegister, 1);

    // In this case the effective clocks will be 500MHz (2us). Meaning that
    // uneven microseconds can not be clocked correctly. In this case the
    // closest delay rounded down will be used.
    // 2^16 / 2 = (2^16)/(2^1) = 2^15 = 0x8000
    const auto extectedArr = 0x8000;
    ASSERT_EQ(m_autoReloadRegister, extectedArr);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

// TODO: Fix
TEST_F(FixtureDelayStm32,
       SynchronousWait_us_SetsPrescalerToOneAndCorrectArrIfNeededBasedOnClockHertzIfRequiredDelayUnven) {
    auto delayStm32 = CreateDelayStm32();

    const uint32_t inputHertz = 1000000;  // 1 MHz => 1us per clock
    m_delayConfig.timerInputFrequencyInHertz = inputHertz;
    delayStm32->SetupConfiguration(std::move(m_delayConfig));

    // The ARR register is assumed ot be 16-bits. At a clock of 1 MHz this means that
    // at 2^16 microseconds the prescaler should be at least 1.
    const auto microsecondsWait = static_cast<uint32_t>(UINT16_MAX) + 2;
    auto delayThread = std::jthread([&]() { delayStm32->SynchronousWait_us(microsecondsWait); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    ASSERT_EQ(m_prescalerRegister, 1);

    // In this case the effective clocks will be 1MHz (2us). Meaning that
    // uneven microseconds can not be clocked correctly. In this case the
    // closed delay rounded down will be used.
    // (2^16 + 1) / 2 = (2^16)/(2^1) + 1/2 =(rounded) 2^15 = 0x8000
    const auto extectedArr = 0x8000;
    ASSERT_EQ(m_autoReloadRegister, extectedArr);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

TEST_F(FixtureDelayStm32, SynchronousWait_us_SetsPrescalerAndArrCorrectlyForOneSecondDelay) {
    auto delayStm32 = CreateDelayStm32();

    const uint32_t inputHertz = 20000000;  // 20 MHz => 0.05us per clock
    m_delayConfig.timerInputFrequencyInHertz = inputHertz;
    delayStm32->SetupConfiguration(std::move(m_delayConfig));

    const auto microsecondsWait = 1000000;  // 1 second
    auto delayThread = std::jthread([&]() { delayStm32->SynchronousWait_us(microsecondsWait); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    // Assuming an infinite ARR bit-length would require (1,000,000 / 0.05 = 20,000,000) clocks.
    // The maximum ARR register is 16-bits (assumption) which is 65,535 clocks which is less than 20,000,000.
    // We would need a microseconds-per-clock of (1,000,000 / 65,535 ~= 15.26us). Where 1,000,000 here is the delay in
    // us. This results in a minimum required prescaler of ((1,000,000 / 65,535) / 0.05 ~= 305.18044) It is assumed that
    // a prescaler will be rounded up and this a prescaler of 306 is expected

    // -1 to take the +1 of the stm32 into account;
    const uint32_t expectedPrescalerRegisterValue = 306 - 1;
    ASSERT_EQ(m_prescalerRegister, expectedPrescalerRegisterValue);

    // At a prescaler of 306 and a clock of 20 MHz the effective clock is
    // (20,000,000 / 306 =(int division) 65,359 Hertz)
    // The clocks-per-microseconds is then (65,359 / 1,000,000 = 0.065359)
    // The expected ARR value is then 1,000,000us * 0.065359 = 65,359
    const auto extectedArr = 65359;
    ASSERT_EQ(m_autoReloadRegister, extectedArr);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

TEST_F(FixtureDelayStm32, SynchronousWait_us_ClearsStatusRegisterOnStart) {
    auto delayStm32 = CreateDelayStm32Configured();

    ASSERT_NE(m_statusRegister, 0);

    auto delayThread = std::jthread([&]() { delayStm32->SynchronousWait_us(m_defaultMicrosecondsDelay); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    ASSERT_EQ(m_statusRegister, 0);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

TEST_F(FixtureDelayStm32, SynchronousWait_us_SetsCounterEnableInControlRegisterOnStart) {
    auto delayStm32 = CreateDelayStm32Configured();

    ASSERT_FALSE(m_controlRegister & m_counterEnableMask);

    auto delayThread = std::jthread([&]() { delayStm32->SynchronousWait_us(m_defaultMicrosecondsDelay); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    ASSERT_TRUE(m_controlRegister & m_counterEnableMask);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();
}

// TODO: Fix
TEST_F(FixtureDelayStm32, SynchronousWait_us_ReturnsTrueWhenUpdateInterruptFlagInStatusRegisterIsSet) {
    auto delayStm32 = CreateDelayStm32Configured();

    auto result = false;

    auto delayThread = std::jthread([&]() { result = delayStm32->SynchronousWait_us(m_defaultMicrosecondsDelay); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();

    ASSERT_TRUE(result);
}

TEST_F(FixtureDelayStm32, SynchronousWait_us_WaitsForInterruptFlagToBeSetBeforeReturning) {
    auto delayStm32 = CreateDelayStm32Configured();

    auto result = false;

    auto delayThread = std::jthread([&]() { result = delayStm32->SynchronousWait_us(m_defaultMicrosecondsDelay); });
    // Make sure the thread is active
    while (!delayThread.joinable()) {
    }

    {
        // Wait for SynchronousWait_us to have started the timer
        usleep(m_microsecondsWaitAfterJoinable);
    }

    // Note that this function should just wait for m_updateInterruptMask to be set.
    // The m_defaultMicrosecondsDelay value only affects values in the real registers in the uController.
    ASSERT_FALSE(result);

    m_statusRegister |= m_updateInterruptMask;
    delayThread.join();

    ASSERT_TRUE(result);
}

//---------------
// IDelayStm32
//---------------

//====================
// SetupConfiguration()
//====================

TEST_F(FixtureDelayStm32, SetupConfiguration_ReturnsFalseIfStatusRegisterIsNull) {
    auto iDelayStm32 = CreateIDelayStm32();

    m_delayConfig.pStatusRegister = nullptr;

    ASSERT_FALSE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
}

TEST_F(FixtureDelayStm32, SetupConfiguration_ReturnsFalseIfAutoReloadRegisterIsNull) {
    auto iDelayStm32 = CreateIDelayStm32();

    m_delayConfig.pAutoReloadRegister = nullptr;

    ASSERT_FALSE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
}

TEST_F(FixtureDelayStm32, SetupConfiguration_ReturnsFalseIfControlRegisterIsNull) {
    auto iDelayStm32 = CreateIDelayStm32();

    m_delayConfig.pControlRegister = nullptr;

    ASSERT_FALSE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
}

TEST_F(FixtureDelayStm32, SetupConfiguration_ReturnsFalseIfPrescalerRegisterIsNull) {
    auto iDelayStm32 = CreateIDelayStm32();

    m_delayConfig.pPrescalerRegister = nullptr;

    ASSERT_FALSE(iDelayStm32->SetupConfiguration(std::move(m_delayConfig)));
}

FUZZ_TEST_F(FixtureDelayStm32FuzzTests, SetupConfiguration_ReturnsFalseIfUpdateInterruptMaskDoesNotHaveOneBitSet);

FUZZ_TEST_F(FixtureDelayStm32FuzzTests, SetupConfiguration_ReturnsTrueIfUpdateInterruptMaskHasOneBitSet)
    .WithDomains(InRange(0, 31));

FUZZ_TEST_F(FixtureDelayStm32FuzzTests, SetupConfiguration_ReturnsFalseIfCounterEnableMaskDoesNotHaveOneBitSet);

FUZZ_TEST_F(FixtureDelayStm32FuzzTests, SetupConfiguration_ReturnsTrueIfCounterEnableMaskHasOneBitSet)
    .WithDomains(InRange(0, 31));

//====================
// GetConfiguration()
//====================

TEST_F(FixtureDelayStm32, GetConfiguration_ReturnsFalseIfPrescalerRegisterIsNull) {
    auto iDelayStm32 = CreateIDelayStm32();

    auto expectedConfig = m_delayConfig;

    iDelayStm32->SetupConfiguration(std::move(m_delayConfig));

    ASSERT_EQ(iDelayStm32->GetConfiguration(), expectedConfig);
}

}  // namespace DelayStm32Testing

}  // namespace HAL::STM32
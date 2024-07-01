#include "GPIOInputStm32.h"
//-----
#include "fuzztest/fuzztest.h"
#include "fuzztest/googletest_fixture_adapter.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace HAL::STM32 {

namespace GPIOInputStm32Testing {

class FixtureGPIOInputStm32 : public Test {
   public:
    FixtureGPIOInputStm32()
        : m_inputRegister(0),
          m_gPIOInputConfig({.pInputRegister = &m_inputRegister, .pinMask = 0x4, .debounceCount = 3}) {
        std::cout << "Creating FixtureGPIOInputStm32" << std::endl;
    }

    auto CreateIGPIOInputStm32() -> std::unique_ptr<STM32::IGPIOInput> { return std::make_unique<STM32::GPIOInput>(); }

    volatile uint32_t m_inputRegister;

    GPIOInputConfig m_gPIOInputConfig;
};

class FixtureGPIOInputStm32FuzzTests : public PerFuzzTestFixtureAdapter<FixtureGPIOInputStm32> {
   public:
    //--------------------
    // SetupConfiguration()
    //--------------------

    void SetupConfiguration_ReturnsTrueIfInputRegisterIsNotNullptr(uint32_t inputRegister) {
        auto iGPIOInputStm32 = CreateIGPIOInputStm32();

        m_gPIOInputConfig.pInputRegister = &inputRegister;

        EXPECT_TRUE(iGPIOInputStm32->SetupConfiguration(std::move(m_gPIOInputConfig)));
    }

    void SetupConfiguration_ReturnsTrueIfMaskHasOneBitSet(uint8_t shifts) {
        auto iGPIOInputStm32 = CreateIGPIOInputStm32();

        uint32_t pinMask = 0x1 << shifts;

        m_gPIOInputConfig.pinMask = pinMask;

        EXPECT_TRUE(iGPIOInputStm32->SetupConfiguration(std::move(m_gPIOInputConfig)));
    }

    void SetupConfiguration_ReturnsFalseIfMaskDoesNotHaveOneBitSet(uint32_t pinMask) {
        if ((pinMask != 0) && (((pinMask & (pinMask - 1)) == 0))) {
            // By setting two bits, there will always be more than one bit set.
            // Even if pinMask is 0x10 or 0x01
            pinMask |= 0x11;
        }

        auto iGPIOInputStm32 = CreateIGPIOInputStm32();

        m_gPIOInputConfig.pinMask = pinMask;

        EXPECT_FALSE(iGPIOInputStm32->SetupConfiguration(std::move(m_gPIOInputConfig)));
    }

    //--------------------
    // GetConfiguration()
    //--------------------

    void GetConfiguration_ReturnsCorrectConfigFuzz(uint32_t inputRegister, uint8_t shifts, uint8_t debounceCount) {
        auto iGPIOInputStm32 = CreateIGPIOInputStm32();

        uint32_t pinMask = 0x1 << shifts;

        m_gPIOInputConfig.pInputRegister = &inputRegister;
        m_gPIOInputConfig.pinMask = pinMask;
        m_gPIOInputConfig.debounceCount = debounceCount;

        const auto expectedConfig = m_gPIOInputConfig;
        iGPIOInputStm32->SetupConfiguration(std::move(m_gPIOInputConfig));

        EXPECT_EQ(iGPIOInputStm32->GetConfiguration(), expectedConfig);
    }
};

//---------------
// IGPIOInputStm32
//---------------

//====================
// SetupConfiguration()
//====================

TEST_F(FixtureGPIOInputStm32, SetupConfiguration_ReturnsFalseIfInputRegisterIsNullptr) {
    auto iGPIOInputStm32 = CreateIGPIOInputStm32();

    m_gPIOInputConfig.pInputRegister = nullptr;

    EXPECT_FALSE(iGPIOInputStm32->SetupConfiguration(std::move(m_gPIOInputConfig)));
}

FUZZ_TEST_F(FixtureGPIOInputStm32FuzzTests, SetupConfiguration_ReturnsTrueIfInputRegisterIsNotNullptr);

FUZZ_TEST_F(FixtureGPIOInputStm32FuzzTests, SetupConfiguration_ReturnsTrueIfMaskHasOneBitSet)
    .WithDomains(InRange(0, 31));

FUZZ_TEST_F(FixtureGPIOInputStm32FuzzTests, SetupConfiguration_ReturnsFalseIfMaskDoesNotHaveOneBitSet);

//====================
// GetConfiguration()
//====================

FUZZ_TEST_F(FixtureGPIOInputStm32FuzzTests, GetConfiguration_ReturnsCorrectConfigFuzz)
    .WithDomains(Arbitrary<uint32_t>(), InRange(0, 31), Arbitrary<uint8_t>());

//====================
// DebounceInterruptHandler()
//====================

TEST_F(FixtureGPIOInputStm32, DebounceInterruptHandler_ReturnsFalseIfConfigNotSet) {
    auto iGPIOInputStm32 = CreateIGPIOInputStm32();
    EXPECT_FALSE(iGPIOInputStm32->DebounceInterruptHandler());
}

TEST_F(FixtureGPIOInputStm32, DebounceInterruptHandler_ReturnsTrueIfConfigSet) {
    auto iGPIOInputStm32 = CreateIGPIOInputStm32();
    iGPIOInputStm32->SetupConfiguration(std::move(m_gPIOInputConfig));
    EXPECT_TRUE(iGPIOInputStm32->DebounceInterruptHandler());
}

TEST_F(FixtureGPIOInputStm32,
       DebounceInterruptHandler_SetsPinStateToTrueIfDebouncedSpecifiedTimeWithConstantHighSignal) {
    STM32::GPIOInput gPIOInputStm32;

    m_gPIOInputConfig.debounceCount = 3;

    auto localGPIOInputConfig = m_gPIOInputConfig;
    gPIOInputStm32.SetupConfiguration(std::move(localGPIOInputConfig));

    // Set the pin to high
    m_inputRegister |= m_gPIOInputConfig.pinMask;

    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 1st count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 2nd count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 3rd count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 4th count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_TRUE(gPIOInputStm32.GetState());

    // 5th count should still result in the same signal if the signal itself didn't change
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_TRUE(gPIOInputStm32.GetState());
}

TEST_F(FixtureGPIOInputStm32,
       DebounceInterruptHandler_SetsPinStateToFalseIfDebouncedSpecifiedTimeWithConstantLowSignal) {
    STM32::GPIOInput gPIOInputStm32;

    m_gPIOInputConfig.debounceCount = 3;

    auto localGPIOInputConfig = m_gPIOInputConfig;
    gPIOInputStm32.SetupConfiguration(std::move(localGPIOInputConfig));

    {
        // Set pin state to high
        m_inputRegister |= m_gPIOInputConfig.pinMask;
        gPIOInputStm32.DebounceInterruptHandler();
        gPIOInputStm32.DebounceInterruptHandler();
        gPIOInputStm32.DebounceInterruptHandler();
        gPIOInputStm32.DebounceInterruptHandler();
    }

    // Set the pin to high
    m_inputRegister &= ~m_gPIOInputConfig.pinMask;

    ASSERT_TRUE(gPIOInputStm32.GetState());

    // 1st count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_TRUE(gPIOInputStm32.GetState());

    // 2nd count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_TRUE(gPIOInputStm32.GetState());

    // 3rd count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_TRUE(gPIOInputStm32.GetState());

    // 4th count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 5th count should still result in the same signal if the signal itself didn't change
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());
}

TEST_F(FixtureGPIOInputStm32,
       DebounceInterruptHandler_DoesNotChangePinStateIfSignalChangesBeforeDebouncedSpecifiedCount) {
    STM32::GPIOInput gPIOInputStm32;

    m_gPIOInputConfig.debounceCount = 3;

    auto localGPIOInputConfig = m_gPIOInputConfig;
    gPIOInputStm32.SetupConfiguration(std::move(localGPIOInputConfig));

    // Set the pin to high
    m_inputRegister |= m_gPIOInputConfig.pinMask;

    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 1st count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 2nd count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());

    // 3rd count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());

    // Set the pin to low to break the debounce count
    m_inputRegister &= ~m_gPIOInputConfig.pinMask;

    // 4th count
    gPIOInputStm32.DebounceInterruptHandler();
    ASSERT_FALSE(gPIOInputStm32.GetState());
}

}  // namespace GPIOInputStm32Testing

}  // namespace HAL::STM32
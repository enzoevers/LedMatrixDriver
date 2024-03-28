#include "GPIOOutputStm32.h"
//-----
#include "fuzztest/fuzztest.h"
#include "fuzztest/googletest_fixture_adapter.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace GPIOOutputStm32Testing {

class FixtureGPIOOutputStm32 : public Test {
   public:
    FixtureGPIOOutputStm32() : m_gPIOOutputConfigStm32({&m_outputRegister, 0x1}) {
        std::cout << "Creating FixtureGPIOOutputStm32" << std::endl;
    }

    auto CreateIGPIOOutputStm32() -> std::unique_ptr<IGPIOOutputStm32> { return std::make_unique<GPIOOutputStm32>(); }

    GPIOOutputConfigStm32 m_gPIOOutputConfigStm32;

    uint32_t m_outputRegister;
};

class FixtureGPIOOutputStm32FuzzTests : public PerFuzzTestFixtureAdapter<FixtureGPIOOutputStm32> {
   public:
    //--------------------
    // SetupConfiguration()
    //--------------------

    void SetupConfiguration_ReturnsTrueIfOutputRegisterIsNotNullptr(uint32_t outputRegister) {
        auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();

        m_gPIOOutputConfigStm32.pOutputRegister = &outputRegister;

        EXPECT_TRUE(iGPIOOutputStm32->SetupConfiguration(std::move(m_gPIOOutputConfigStm32)));
    }

    void SetupConfiguration_ReturnsTrueIfMaskHasOneBitSet(uint8_t shifts) {
        auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();

        uint32_t pinMask = 0x1 << shifts;

        m_gPIOOutputConfigStm32.pinMask = pinMask;

        EXPECT_TRUE(iGPIOOutputStm32->SetupConfiguration(std::move(m_gPIOOutputConfigStm32)));
    }

    void SetupConfiguration_ReturnsFalseIfMaskDoesNotHaveOneBitSet(uint32_t pinMask) {
        if ((pinMask != 0) && (((pinMask & (pinMask - 1)) == 0))) {
            // By setting two bits, there will always be more than one bit set.
            // Even if pinMask is 0x10 or 0x01
            pinMask |= 0x11;
        }

        auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();

        m_gPIOOutputConfigStm32.pinMask = pinMask;

        EXPECT_FALSE(iGPIOOutputStm32->SetupConfiguration(std::move(m_gPIOOutputConfigStm32)));
    }

    //--------------------
    // GetConfiguration()
    //--------------------

    void GetConfiguration_ReturnsCorrectConfigFuzz(uint32_t outputRegister, uint8_t shifts) {
        auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();

        uint32_t pinMask = 0x1 << shifts;

        m_gPIOOutputConfigStm32.pOutputRegister = &outputRegister;
        m_gPIOOutputConfigStm32.pinMask = pinMask;

        const auto expectedConfig = m_gPIOOutputConfigStm32;
        iGPIOOutputStm32->SetupConfiguration(std::move(m_gPIOOutputConfigStm32));

        EXPECT_EQ(iGPIOOutputStm32->GetConfiguration(), expectedConfig);
    }
};

//---------------
// IGPIOOutputStm32
//---------------

//====================
// SetupConfiguration()
//====================

TEST_F(FixtureGPIOOutputStm32, SetupConfiguration_ReturnsFalseIfOutputRegisterIsNullptr) {
    auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();

    m_gPIOOutputConfigStm32.pOutputRegister = nullptr;

    EXPECT_FALSE(iGPIOOutputStm32->SetupConfiguration(std::move(m_gPIOOutputConfigStm32)));
}

FUZZ_TEST_F(FixtureGPIOOutputStm32FuzzTests, SetupConfiguration_ReturnsTrueIfOutputRegisterIsNotNullptr);

FUZZ_TEST_F(FixtureGPIOOutputStm32FuzzTests, SetupConfiguration_ReturnsTrueIfMaskHasOneBitSet)
    .WithDomains(InRange(0, 31));

FUZZ_TEST_F(FixtureGPIOOutputStm32FuzzTests, SetupConfiguration_ReturnsFalseIfMaskDoesNotHaveOneBitSet);

//====================
// GetConfiguration()
//====================

FUZZ_TEST_F(FixtureGPIOOutputStm32FuzzTests, GetConfiguration_ReturnsCorrectConfigFuzz)
    .WithDomains(Arbitrary<uint32_t>(), InRange(0, 31));

}  // namespace GPIOOutputStm32Testing
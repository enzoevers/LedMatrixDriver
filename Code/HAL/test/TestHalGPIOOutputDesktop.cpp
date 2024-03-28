#include "GPIOOutputDesktop.h"
//-----
#include "fuzztest/fuzztest.h"
#include "fuzztest/googletest_fixture_adapter.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace GPIOOutputDesktopTesting {

class FixtureGPIOOutputDesktop : public Test {
   public:
    FixtureGPIOOutputDesktop() : m_gPIOOutputConfigDesktop({&m_outputRegister, 0x1}) {}

    auto CreateIGPIOOutputDesktop() -> std::unique_ptr<IGPIOOutputDesktop> {
        return std::make_unique<GPIOOutputDesktop>();
    }

    GPIOOutputConfigDesktop m_gPIOOutputConfigDesktop;

    uint32_t m_outputRegister;
};

class FixtureGPIOOutputDesktopFuzzTests : public PerFuzzTestFixtureAdapter<FixtureGPIOOutputDesktop> {
   public:
    //--------------------
    // SetupConfiguration()
    //--------------------

    void SetupConfiguration_DoesNotThrowIfOutputRegisterIsNotNullptr(uint32_t outputRegister) {
        auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();

        m_gPIOOutputConfigDesktop.pOutputRegister = &outputRegister;

        ASSERT_NO_THROW(iGPIOOutputDesktop->SetupConfiguration(std::move(m_gPIOOutputConfigDesktop)));
    }

    void SetupConfiguration_DoesNotThrowIfMaskHasOneBitSet(uint8_t shifts) {
        auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();

        uint32_t pinMask = 0x1 << shifts;

        m_gPIOOutputConfigDesktop.pinMask = pinMask;

        ASSERT_NO_THROW(iGPIOOutputDesktop->SetupConfiguration(std::move(m_gPIOOutputConfigDesktop)));
    }

    void SetupConfiguration_ThrowsIfMaskDoesNotHaveOneBitSet(uint32_t pinMask) {
        if ((pinMask != 0) && (((pinMask & (pinMask - 1)) == 0))) {
            // By setting two bits, there will always be more than one bit set.
            // Even if pinMask is 0x10 or 0x01
            pinMask |= 0x11;
        }

        auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();

        m_gPIOOutputConfigDesktop.pinMask = pinMask;

        EXPECT_THAT([&]() { iGPIOOutputDesktop->SetupConfiguration(std::move(m_gPIOOutputConfigDesktop)); },
                    ThrowsMessage<std::invalid_argument>("Only 1 set bit allowed"));
    }

    //--------------------
    // GetConfiguration()
    //--------------------

    void GetConfiguration_ReturnsCorrectConfigFuzz(uint32_t outputRegister, uint8_t shifts) {
        auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();

        uint32_t pinMask = 0x1 << shifts;

        m_gPIOOutputConfigDesktop.pOutputRegister = &outputRegister;
        m_gPIOOutputConfigDesktop.pinMask = pinMask;

        const auto expectedConfig = m_gPIOOutputConfigDesktop;
        iGPIOOutputDesktop->SetupConfiguration(std::move(m_gPIOOutputConfigDesktop));

        EXPECT_EQ(iGPIOOutputDesktop->GetConfiguration(), expectedConfig);
    }
};

//====================
// SetupConfiguration()
//====================

TEST_F(FixtureGPIOOutputDesktop, SetupConfiguration_ThrowsIfOutputRegisterIsNullptr) {
    auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();

    m_gPIOOutputConfigDesktop.pOutputRegister = nullptr;

    EXPECT_THAT([&]() { iGPIOOutputDesktop->SetupConfiguration(std::move(m_gPIOOutputConfigDesktop)); },
                ThrowsMessage<std::invalid_argument>("No nullptr allowed"));
}

FUZZ_TEST_F(FixtureGPIOOutputDesktopFuzzTests, SetupConfiguration_DoesNotThrowIfOutputRegisterIsNotNullptr);

FUZZ_TEST_F(FixtureGPIOOutputDesktopFuzzTests, SetupConfiguration_DoesNotThrowIfMaskHasOneBitSet)
    .WithDomains(InRange(0, 31));

FUZZ_TEST_F(FixtureGPIOOutputDesktopFuzzTests, SetupConfiguration_ThrowsIfMaskDoesNotHaveOneBitSet);

//====================
// GetConfiguration()
//====================

FUZZ_TEST_F(FixtureGPIOOutputDesktopFuzzTests, GetConfiguration_ReturnsCorrectConfigFuzz)
    .WithDomains(Arbitrary<uint32_t>(), InRange(0, 31));

}  // namespace GPIOOutputDesktopTesting
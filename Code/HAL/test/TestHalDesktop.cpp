#include "GPIOOutputDesktop.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace GPIOOutputDesktopTesting {

class FixtureGPIOOutputDesktop : public Test {
   public:
    FixtureGPIOOutputDesktop() : m_outputRegister(0), m_pinMask(0x1 << 13) {}

    auto CreateIGPIOOutput() -> std::unique_ptr<IGPIOOutput> { return std::make_unique<GPIOOutputDesktop>(); }

    auto CreateIGPIOOutputDesktop() -> std::unique_ptr<IGPIOOutputDesktop> {
        return std::make_unique<GPIOOutputDesktop>();
    }

    auto CreateGPIOOutputDesktop() -> std::unique_ptr<GPIOOutputDesktop> {
        return std::make_unique<GPIOOutputDesktop>();
    }

    auto SetOutputRegisterAndPinMask(IGPIOOutputDesktop& iGPIOOutputDesktop) -> void {
        iGPIOOutputDesktop.SetOutputRegister(&m_outputRegister);
        iGPIOOutputDesktop.SetPinMask(m_pinMask);
    }

    uint32_t m_outputRegister = 0;
    uint32_t m_pinMask = 1 << 13;
};

//---------------
// IGPIOOutputDesktop
//---------------

//====================
// SetOutputRegister()
//====================

TEST_F(FixtureGPIOOutputDesktop, SetOutputRegister_ThrowsIfNullptr) {
    auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();

    EXPECT_THAT([&]() { iGPIOOutputDesktop->SetOutputRegister(nullptr); },
                ThrowsMessage<std::invalid_argument>("No nullptr allowed"));
}

void SetOutputRegister_DoesNotThrowIfNotNullptr(uint32_t outputRegister) {
    auto iGPIOOutputDesktop = std::make_unique<GPIOOutputDesktop>();

    iGPIOOutputDesktop->SetOutputRegister(&outputRegister);
}
FUZZ_TEST(TestGPIOOutputDesktop, SetOutputRegister_DoesNotThrowIfNotNullptr);

//====================
// SetOutputRegister() + GetOutputRegister()
//====================

void SetOutputRegisterIsReflectedInGetOutputRegisterFuzz(uint32_t outputRegister) {
    auto gPIOOutputDesktop = std::make_unique<GPIOOutputDesktop>();

    gPIOOutputDesktop->SetOutputRegister(&outputRegister);

    EXPECT_EQ(gPIOOutputDesktop->GetOutputRegister(), &outputRegister);
}
FUZZ_TEST(TestGPIOOutputDesktop, SetOutputRegisterIsReflectedInGetOutputRegisterFuzz);

//====================
// SetPinMask()
//====================

void SetPinMask_ThrowsIfMaskDoesNotHaveOneBitSet(uint32_t pinMask) {
    if ((pinMask != 0) && (((pinMask & (pinMask - 1)) == 0))) {
        // By setting two bits, there will always be more than one bit set.
        // Even if pinMask is 0x10 or 0x01
        pinMask |= 0x11;
    }

    auto gPIOOutputDesktop = std::make_unique<GPIOOutputDesktop>();

    EXPECT_THAT([&]() { gPIOOutputDesktop->SetPinMask(pinMask); },
                ThrowsMessage<std::invalid_argument>("Only 1 set bit allowed"));
}
FUZZ_TEST(TestGPIOOutputDesktop, SetPinMask_ThrowsIfMaskDoesNotHaveOneBitSet);

void SetPinMask_DoesNotThrowIfMaskHasOneBitSet(uint8_t shifts) {
    auto gPIOOutputDesktop = std::make_unique<GPIOOutputDesktop>();

    uint32_t pinMask = 0x1 << shifts;
    gPIOOutputDesktop->SetPinMask(pinMask);
}
FUZZ_TEST(TestGPIOOutputDesktop, SetPinMask_DoesNotThrowIfMaskHasOneBitSet).WithDomains(InRange(0, 31));

//====================
// SetPinMask() + GetPinMask()
//====================

void SetPinMaskIsReflectedInGetPinMask(uint8_t shifts) {
    auto gPIOOutputDesktop = std::make_unique<GPIOOutputDesktop>();

    uint32_t pinMask = 0x1 << shifts;
    gPIOOutputDesktop->SetPinMask(pinMask);

    EXPECT_EQ(gPIOOutputDesktop->GetPinMask(), pinMask);
}
FUZZ_TEST(TestGPIOOutputDesktop, SetPinMaskIsReflectedInGetPinMask).WithDomains(InRange(0, 31));

}  // namespace GPIOOutputDesktopTesting
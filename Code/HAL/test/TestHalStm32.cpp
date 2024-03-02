#include "GPIOOutputStm32.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace GPIOOutputStm32Testing {

class FixtureGPIOOutputStm32 : public Test {
   public:
    FixtureGPIOOutputStm32() : m_outputRegister(0), m_pinMask(0x1 << 13) {}

    auto CreateIGPIOOutput() -> std::unique_ptr<IGPIOOutput> { return std::make_unique<GPIOOutputStm32>(); }

    auto CreateIGPIOOutputStm32() -> std::unique_ptr<IGPIOOutputStm32> { return std::make_unique<GPIOOutputStm32>(); }

    auto CreateGPIOOutputStm32() -> std::unique_ptr<GPIOOutputStm32> { return std::make_unique<GPIOOutputStm32>(); }

    auto SetOutputRegisterAndPinMask(IGPIOOutputStm32& iGPIOOutputStm32) -> void {
        iGPIOOutputStm32.SetOutputRegister(&m_outputRegister);
        iGPIOOutputStm32.SetPinMask(m_pinMask);
    }

    uint32_t m_outputRegister;
    uint32_t m_pinMask;
};

//---------------
// IGPIOOutputStm32
//---------------

//====================
// SetOutputRegister()
//====================

TEST_F(FixtureGPIOOutputStm32, SetOutputRegister_ReturnsFalseIfNullptr) {
    auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();

    EXPECT_FALSE(iGPIOOutputStm32->SetOutputRegister(nullptr));
}

void SetOutputRegister_ReturnsTrueIfNotNullptr(uint32_t outputRegister) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    EXPECT_TRUE(iGPIOOutputStm32->SetOutputRegister(&outputRegister));
}
FUZZ_TEST(TestGPIOOutputStm32, SetOutputRegister_ReturnsTrueIfNotNullptr);

//====================
// SetOutputRegister() + GetOutputRegister()
//====================

void SetOutputRegisterIsReflectedInGetOutputRegisterFuzz(uint32_t outputRegister) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    iGPIOOutputStm32->SetOutputRegister(&outputRegister);

    EXPECT_EQ(iGPIOOutputStm32->GetOutputRegister(), &outputRegister);
}
FUZZ_TEST(TestGPIOOutputStm32, SetOutputRegisterIsReflectedInGetOutputRegisterFuzz);

//====================
// SetPinMask()
//====================

void SetPinMask_ReturnsFalseIfMaskDoesNotHaveOneBitSet(uint32_t pinMask) {
    if ((pinMask != 0) && (((pinMask & (pinMask - 1)) == 0))) {
        // By setting two bits, there will always be more than one bit set.
        // Even if pinMask is 0x10 or 0x01
        pinMask |= 0x11;
    }

    auto gPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    EXPECT_FALSE(gPIOOutputStm32->SetPinMask(pinMask));
}
FUZZ_TEST(TestGPIOOutputStm32, SetPinMask_ReturnsFalseIfMaskDoesNotHaveOneBitSet);

void SetPinMask_ReturnsTrueIfMaskHasOneBitSet(uint8_t shifts) {
    auto gPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    uint32_t pinMask = 0x1 << shifts;
    EXPECT_TRUE(gPIOOutputStm32->SetPinMask(pinMask));
}
FUZZ_TEST(TestGPIOOutputStm32, SetPinMask_ReturnsTrueIfMaskHasOneBitSet).WithDomains(InRange(0, 31));

//====================
// SetPinMask() + GetPinMask()
//====================

void SetPinMaskIsReflectedInGetPinMaskFuzz(uint8_t shifts) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    uint32_t pinMask = 0x1 << shifts;
    iGPIOOutputStm32->SetPinMask(pinMask);

    EXPECT_EQ(iGPIOOutputStm32->GetPinMask(), pinMask);
}
FUZZ_TEST(TestGPIOOutputStm32, SetPinMaskIsReflectedInGetPinMaskFuzz).WithDomains(InRange(0, 31));

}  // namespace GPIOOutputStm32Testing
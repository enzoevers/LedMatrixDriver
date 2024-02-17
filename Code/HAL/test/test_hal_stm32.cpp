#include "GPIOOutputStm32.h"
#include "fuzztest/fuzztest.h"
#include "gtest/gtest.h"

using namespace testing;

class FixtureGPIOOutputStm32 : public Test {
   public:
    auto CreateIGPIOOutput() -> std::unique_ptr<IGPIOOutput> { return std::make_unique<GPIOOutputStm32>(); }

    auto CreateIGPIOOutputStm32() -> std::unique_ptr<IGPIOOutputStm32> { return std::make_unique<GPIOOutputStm32>(); }
};

//---------------
// IGPIOOutput
//---------------

//====================
// SetState() + GetState()
//====================

TEST_F(FixtureGPIOOutputStm32, SetStateIsReflectedInGetState_True) {
    auto iGPIOOutput = CreateIGPIOOutput();

    iGPIOOutput->SetState(true);

    EXPECT_TRUE(iGPIOOutput->GetState());
}

TEST_F(FixtureGPIOOutputStm32, SetStateIsReflectedInGetState_False) {
    auto iGPIOOutput = CreateIGPIOOutput();

    iGPIOOutput->SetState(false);

    EXPECT_FALSE(iGPIOOutput->GetState());
}

//---------------
// IGPIOOutputStm32
//---------------

//====================
// SetOutputRegister() + GetOutputRegister()
//====================

TEST_F(FixtureGPIOOutputStm32, SetOutputRegisterIsReflectedInGetOutputRegister) {
    auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();
    std::unique_ptr<uint32_t> pOutputRegister;

    iGPIOOutputStm32->SetOutputRegister(pOutputRegister.get());

    EXPECT_EQ(iGPIOOutputStm32->GetOutputRegister(), pOutputRegister.get());
}

//====================
// SetPinMask() + GetPinMask()
//====================

TEST_F(FixtureGPIOOutputStm32, SetPinMaskIsReflectedInGetPinMask) {
    auto iGPIOOutputStm32 = CreateIGPIOOutputStm32();
    uint32_t pinMask;

    iGPIOOutputStm32->SetPinMask(pinMask);

    EXPECT_EQ(iGPIOOutputStm32->GetPinMask(), pinMask);
}

void SetPinMaskIsReflectedInGetPinMaskFuzz(uint32_t pinMask) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    iGPIOOutputStm32->SetPinMask(pinMask);

    EXPECT_EQ(iGPIOOutputStm32->GetPinMask(), pinMask);
}
FUZZ_TEST(GPIOOutputStm32, SetPinMaskIsReflectedInGetPinMaskFuzz);
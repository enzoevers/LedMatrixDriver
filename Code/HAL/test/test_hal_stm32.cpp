#include "GPIOOutputStm32.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace fuzztest;

namespace GPIOOutputStm32Testing {

class FixtureGPIOOutputStm32 : public Test {
   public:
    auto CreateIGPIOOutput() -> std::unique_ptr<IGPIOOutput> { return std::make_unique<GPIOOutputStm32>(); }

    auto CreateIGPIOOutputStm32() -> std::unique_ptr<IGPIOOutputStm32> { return std::make_unique<GPIOOutputStm32>(); }

    auto CreateGPIOOutputStm32() -> std::unique_ptr<GPIOOutputStm32> { return std::make_unique<GPIOOutputStm32>(); }
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

TEST_F(FixtureGPIOOutputStm32, SetStateIsReflectedInGetState_Toggle) {
    auto gPIOOutput = CreateGPIOOutputStm32();

    uint32_t outputRegister = 0;
    gPIOOutput->SetOutputRegister(&outputRegister);

    gPIOOutput->SetState(true);
    EXPECT_TRUE(gPIOOutput->GetState());

    gPIOOutput->SetState(false);
    EXPECT_FALSE(gPIOOutput->GetState());

    gPIOOutput->SetState(true);
    EXPECT_TRUE(gPIOOutput->GetState());

    gPIOOutput->SetState(false);
    EXPECT_FALSE(gPIOOutput->GetState());
}

//---------------
// IGPIOOutputStm32
//---------------

//====================
// SetOutputRegister() + GetOutputRegister()
//====================

// TODO: Make test for no nullptr allowed

void SetOutputRegisterIsReflectedInGetOutputRegisterFuzz(std::unique_ptr<uint32_t> pOutputRegister) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    iGPIOOutputStm32->SetOutputRegister(pOutputRegister.get());

    EXPECT_EQ(iGPIOOutputStm32->GetOutputRegister(), pOutputRegister.get());
}
FUZZ_TEST(GPIOOutputStm32, SetOutputRegisterIsReflectedInGetOutputRegisterFuzz)
    .WithDomains(SmartPointerOf<std::unique_ptr<uint32_t>>(Arbitrary<uint32_t>()));

//====================
// SetPinMask() + GetPinMask()
//====================

void SetPinMaskIsReflectedInGetPinMaskFuzz(uint32_t pinMask) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputStm32>();

    iGPIOOutputStm32->SetPinMask(pinMask);

    EXPECT_EQ(iGPIOOutputStm32->GetPinMask(), pinMask);
}
FUZZ_TEST(GPIOOutputStm32, SetPinMaskIsReflectedInGetPinMaskFuzz);

}  // namespace GPIOOutputStm32Testing
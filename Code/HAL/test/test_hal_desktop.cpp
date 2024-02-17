#include "GPIOOutputDesktop.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace fuzztest;

namespace GPIOOutputDesktopTesting {

class FixtureGPIOOutputDesktop : public Test {
   public:
    auto CreateIGPIOOutput() -> std::unique_ptr<IGPIOOutput> { return std::make_unique<GPIOOutputDesktop>(); }

    auto CreateIGPIOOutputDesktop() -> std::unique_ptr<IGPIOOutputDesktop> {
        return std::make_unique<GPIOOutputDesktop>();
    }

    auto CreateGPIOOutputDesktop() -> std::unique_ptr<GPIOOutputDesktop> {
        return std::make_unique<GPIOOutputDesktop>();
    }
};

//---------------
// IGPIOOutput
//---------------

//====================
// SetState() + GetState()
//====================

TEST_F(FixtureGPIOOutputDesktop, SetStateIsReflectedInGetState_True) {
    auto iGPIOOutput = CreateIGPIOOutput();

    iGPIOOutput->SetState(true);

    EXPECT_TRUE(iGPIOOutput->GetState());
}

TEST_F(FixtureGPIOOutputDesktop, SetStateIsReflectedInGetState_False) {
    auto iGPIOOutput = CreateIGPIOOutput();

    iGPIOOutput->SetState(false);

    EXPECT_FALSE(iGPIOOutput->GetState());
}

TEST_F(FixtureGPIOOutputDesktop, SetStateIsReflectedInGetState_Toggle) {
    auto gPIOOutput = CreateGPIOOutputDesktop();

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
// IGPIOOutputDesktop
//---------------

//====================
// SetOutputRegister() + GetOutputRegister()
//====================

// TODO: Make test for no nullptr allowed

void SetOutputRegisterIsReflectedInGetOutputRegisterFuzz(std::unique_ptr<uint32_t> pOutputRegister) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputDesktop>();

    iGPIOOutputStm32->SetOutputRegister(pOutputRegister.get());

    EXPECT_EQ(iGPIOOutputStm32->GetOutputRegister(), pOutputRegister.get());
}
FUZZ_TEST(GPIOOutputDesktop, SetOutputRegisterIsReflectedInGetOutputRegisterFuzz)
    .WithDomains(SmartPointerOf<std::unique_ptr<uint32_t>>(Arbitrary<uint32_t>()));

//====================
// SetPinMask() + GetPinMask()
//====================

void SetPinMaskIsReflectedInGetPinMaskFuzz(uint32_t pinMask) {
    auto iGPIOOutputStm32 = std::make_unique<GPIOOutputDesktop>();

    iGPIOOutputStm32->SetPinMask(pinMask);

    EXPECT_EQ(iGPIOOutputStm32->GetPinMask(), pinMask);
}
FUZZ_TEST(GPIOOutputDesktop, SetPinMaskIsReflectedInGetPinMaskFuzz);

}  // namespace GPIOOutputDesktopTesting
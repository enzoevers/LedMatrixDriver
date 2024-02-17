#include "GPIOOutputDesktop.h"
#include "gtest/gtest.h"

using namespace testing;

class FixtureGPIOOutputDesktop : public Test {
   public:
    auto CreateIGPIOOutput() -> std::unique_ptr<IGPIOOutput> { return std::make_unique<GPIOOutputDesktop>(); }

    auto CreateIGPIOOutputDesktop() -> std::unique_ptr<IGPIOOutputDesktop> {
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

//---------------
// IGPIOOutputDesktop
//---------------

//====================
// SetOutputRegister() + GetOutputRegister()
//====================

TEST_F(FixtureGPIOOutputDesktop, SetOutputRegisterIsReflectedInGetOutputRegister) {
    auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();
    std::unique_ptr<uint32_t> pOutputRegister;

    iGPIOOutputDesktop->SetOutputRegister(pOutputRegister.get());

    EXPECT_EQ(iGPIOOutputDesktop->GetOutputRegister(), pOutputRegister.get());
}

//====================
// SetPinMask() + GetPinMask()
//====================

TEST_F(FixtureGPIOOutputDesktop, SetPinMaskIsReflectedInGetPinMask) {
    auto iGPIOOutputDesktop = CreateIGPIOOutputDesktop();
    uint32_t pinMask;

    iGPIOOutputDesktop->SetPinMask(pinMask);

    EXPECT_EQ(iGPIOOutputDesktop->GetPinMask(), pinMask);
}
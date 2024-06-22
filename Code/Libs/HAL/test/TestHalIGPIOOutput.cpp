#include "GPIOOutputStm32.h"
//-----
#include <tuple>

#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace HAL {

namespace IGPIOOutputTesting {

class FixtureIGPIOOutput : public Test {
   public:
    FixtureIGPIOOutput() : gPIOOutputTargets(std::make_tuple(STM32::GPIOOutput())) {
        InitializeIGPIOOutputVec();
        AssertCorrectInitialized(gPIOOutputTargets);
    }

    auto InitializeIGPIOOutputVec() -> void {
        std::apply([this](auto&&... target) { ((iGPIOOutputVec.emplace_back(&target)), ...); }, gPIOOutputTargets);
    }

    template <class T>
    auto AssertCorrectInitialized(T /*dummy*/) -> void {
        ASSERT_EQ(iGPIOOutputVec.size(), std::tuple_size<T>{});
    }

    auto SetupWorkableGPIOOutput(IGPIOOutput* target) -> void {
        if (auto _target = dynamic_cast<STM32::GPIOOutput*>(target)) {
            std::cout << "GPIOOutputStm32" << std::endl;
            m_outputRegister = 0;
            ASSERT_TRUE(_target->SetupConfiguration({&m_outputRegister, m_pinMask}));
        } else {
            std::cout << "Invalid target" << std::endl;
        }
    }

    std::vector<IGPIOOutput*> iGPIOOutputVec;
    std::tuple<STM32::GPIOOutput> gPIOOutputTargets;

    // For:
    //  GPIOOutputStm32
    uint32_t m_outputRegister = 0;
    uint32_t m_pinMask = 1 << 13;
};

//---------------
// IGPIOOutput
//---------------

//====================
// SetState()
//====================

TEST_F(FixtureIGPIOOutput, SetState_ReturnsFalseIfNoOutputRegisterSet) {
    for (auto& iGPIOOutput : iGPIOOutputVec) {
        EXPECT_FALSE(iGPIOOutput->SetState(true));
    }
}

//====================
// GetState()
//====================

TEST_F(FixtureIGPIOOutput, GetState_ReturnsFalseIfNoOutputRegisterSet) {
    for (auto& iGPIOOutput : iGPIOOutputVec) {
        EXPECT_FALSE(iGPIOOutput->GetState());
    }
}

//====================
// SetState() + GetState()
//====================

TEST_F(FixtureIGPIOOutput, SetStateIsReflectedInGetState_True) {
    for (auto& iGPIOOutput : iGPIOOutputVec) {
        SetupWorkableGPIOOutput(iGPIOOutput);
        iGPIOOutput->SetState(true);
        EXPECT_TRUE(iGPIOOutput->GetState());
    }
}

TEST_F(FixtureIGPIOOutput, SetStateIsReflectedInGetState_False) {
    for (auto& iGPIOOutput : iGPIOOutputVec) {
        SetupWorkableGPIOOutput(iGPIOOutput);
        iGPIOOutput->SetState(false);
        EXPECT_FALSE(iGPIOOutput->GetState());
    }
}

TEST_F(FixtureIGPIOOutput, SetStateIsReflectedInGetState_Toggle) {
    for (auto& iGPIOOutput : iGPIOOutputVec) {
        SetupWorkableGPIOOutput(iGPIOOutput);

        iGPIOOutput->SetState(true);
        EXPECT_TRUE(iGPIOOutput->GetState());

        iGPIOOutput->SetState(false);
        EXPECT_FALSE(iGPIOOutput->GetState());

        iGPIOOutput->SetState(true);
        EXPECT_TRUE(iGPIOOutput->GetState());

        iGPIOOutput->SetState(false);
        EXPECT_FALSE(iGPIOOutput->GetState());
    }
}

}  // namespace IGPIOOutputTesting

}  // namespace HAL
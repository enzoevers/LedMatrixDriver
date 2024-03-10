#include "HanoverOL037A.h"
//-----
#include <tuple>

#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace IDisplayMonochromeTesting {

class FixtureIDisplay : public Test {
   public:
    FixtureIDisplay() : displayTargets(std::make_tuple(HanoverOL037A())) {
        InitializeIDisplayVec();
        AssertCorrectInitialized(displayTargets);
    }

    auto InitializeIDisplayVec() -> void {
        std::apply([this](auto&&... target) { ((iDisplayVec.emplace_back(&target)), ...); }, displayTargets);
    }

    template <class T>
    auto AssertCorrectInitialized(T /*dummy*/) -> void {
        ASSERT_EQ(iDisplayVec.size(), std::tuple_size<T>{});
    }

    auto SetupWorkableDisplay(IDisplay<Monochrome>* target) -> void {
        if (auto _target = dynamic_cast<HanoverOL037A*>(target)) {
            std::cout << "HanoverOL037A" << std::endl;
        } else {
            std::cout << "Invalid target" << std::endl;
        }
    }

    std::vector<IDisplay<Monochrome>*> iDisplayVec;
    std::tuple<HanoverOL037A> displayTargets;
};

}  // namespace IDisplayMonochromeTesting
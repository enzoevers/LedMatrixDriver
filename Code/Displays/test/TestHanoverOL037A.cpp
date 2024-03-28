#include "HanoverOL037A.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace HanoverOL037ATesting {

class FixtureHanoverOL037A : public Test {
   public:
    FixtureHanoverOL037A() {}
};

TEST_F(FixtureHanoverOL037A, dummy) { EXPECT_TRUE(true); }

}  // namespace HanoverOL037ATesting
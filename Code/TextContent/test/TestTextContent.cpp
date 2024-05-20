#include "TextContent.h"
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace TextContentTesting {

class FixtureTextContent : public Test {};

TEST_F(FixtureTextContent, dummy) { ASSERT_TRUE(true); }

}  // namespace TextContentTesting
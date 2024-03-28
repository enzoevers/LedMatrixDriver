#include "DelayDesktop.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace DelayDesktopTesting {

class FixtureDelayDesktop : public Test {};

//---------------
// IDelay
//---------------

//====================
// SynchronousWait_us()
//====================

TEST_F(FixtureDelayDesktop, BusyWait_us_allowsDelayOfZeroUs) {
    auto delayDesktop = std::make_unique<DelayDesktop>();

    delayDesktop->SynchronousWait_us(0);
}

TEST_F(FixtureDelayDesktop, BusyWait_us_WaitsforOneSecondIsOneMillionIsProvided) {
    auto delayDesktop = std::make_unique<DelayDesktop>();

    const auto oneSecondsInMicroSeconds = 1000000;

    const auto startTime = std::chrono::high_resolution_clock::now();
    delayDesktop->SynchronousWait_us(oneSecondsInMicroSeconds);
    const auto endTime = std::chrono::high_resolution_clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    EXPECT_THAT(duration, AllOf(Ge(oneSecondsInMicroSeconds * 0.9), Le(oneSecondsInMicroSeconds * 1.1)));
}

}  // namespace DelayDesktopTesting
#include "GPIOOutputStm32.h"
#include "gtest/gtest.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(GPIOOutputStm32F303xc, gfdhfd) {
    uint32_t* outputRegister = reinterpret_cast<uint32_t*>(0xDEADBEAF);
    uint32_t pinMask = 0x1 << 13;
    auto gpio3 = GPIOOutputStm32(outputRegister, pinMask);
    gpio3.SetState(true);
}
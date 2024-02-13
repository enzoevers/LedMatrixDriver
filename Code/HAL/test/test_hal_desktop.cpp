#include "GPIOOutputDesktop.h"
#include "gtest/gtest.h"

TEST(GPIOOutputDesktop, gfdhfd) {
    auto gpio3 = GPIOOutputDesktop();
    gpio3.SetState(true);
}
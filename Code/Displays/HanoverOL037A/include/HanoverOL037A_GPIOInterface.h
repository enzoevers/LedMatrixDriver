#pragma once

#include "IGPIOOutput.h"

struct HanoverOL037A_GPIOInterface {
    IGPIOOutput& clk;
    IGPIOOutput& clkSelEn;
    IGPIOOutput& data;
    IGPIOOutput& clkEn;
    IGPIOOutput& latch;
    IGPIOOutput& ledOE;  // TODO: later to be replaces with IPWMOutput&
    // IDelayBlocking& delayBlocking; // TODO
};
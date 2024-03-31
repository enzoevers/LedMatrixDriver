#include "BitManipulation.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

#if defined(USE_STM32F303XC)
#include "SetupHardwareStm32f303xc.h"
#endif

#if defined(USE_DESKTOP)
#include "SetupDesktop.h"
#endif

#if defined(USE_STM32)
#include "DelayStm32.h"
#include "GPIOOutputStm32.h"
#endif

#if defined(USE_DESKTOP)
#include <iostream>

#include "DelayDesktop.h"
#include "GPIOOutputDesktop.h"
#endif

#include "HanoverOL037A.h"

int main() {
    HardwareSetup();

#if defined(USE_STM32)
    auto clk = GPIOOutputStm32();
    auto clkSelEn = GPIOOutputStm32();
    // auto data = GPIOOutputStm32();
    // auto clkEn = GPIOOutputStm32();
    // auto latch = GPIOOutputStm32();
    // auto ledOE = GPIOOutputStm32();

    {  // IGPIOOutputStm32 setup
        clk.SetupConfiguration({&GPIOE->ODR, GPIO_ODR_13});
        clkSelEn.SetupConfiguration({&GPIOE->ODR, GPIO_ODR_14});
        // data.SetupConfiguration({&GPIOE->ODR, GPIO_ODR_13});
        // clkEn.SetupConfiguration({&GPIOE->ODR, GPIO_ODR_13});
        // latch.SetupConfiguration({&GPIOE->ODR, GPIO_ODR_13});
        // ledOE.SetupConfiguration({&GPIOE->ODR, GPIO_ODR_13});
    }
    uint32_t tim3Hertz = 8000000;
    auto mainDelay = DelayStm32();
    mainDelay.SetupConfiguration({&TIM3->SR, &TIM3->ARR, &TIM3->CR1, &TIM3->PSC, tim3Hertz, TIM_SR_UIF, TIM_CR1_CEN});
#endif

#if defined(USE_DESKTOP)
    auto clk = GPIOOutputDesktop();
    auto clkSelEn = GPIOOutputDesktop();

    uint32_t outputRegister;
    uint32_t io0Mask = 0x1;
    uint32_t io7Mask = 0x1 << 7;

    {  // IGPIOOutputDesktop setup
        clk.SetupConfiguration({&outputRegister, io0Mask});
        clkSelEn.SetupConfiguration({&outputRegister, io7Mask});
    }

    auto mainDelay = DelayDesktop();
#endif

    // HanoverOL037A_GPIOInterface hanoverOL037A_GPIOInterface = {clk, clkSelEn, data, clkEn, latch, ledOE};

    while (true) {
        clk.SetState(true);
        clkSelEn.SetState(false);
        mainDelay.SynchronousWait_us(1000000);
        clk.SetState(false);
        clkSelEn.SetState(true);
        mainDelay.SynchronousWait_us(1000000);
    }

    return 0;
}
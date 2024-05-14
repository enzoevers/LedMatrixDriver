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
    auto clkEn = GPIOOutputStm32();
    auto clkSelEn = GPIOOutputStm32();
    auto data = GPIOOutputStm32();
    auto latch = GPIOOutputStm32();
    auto ledOE = GPIOOutputStm32();

    {  // IGPIOOutputStm32 setup
        clk.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_10});
        clkEn.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_11});
        clkSelEn.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_12});
        data.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_13});
        latch.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_14});
        ledOE.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_15});
    }
    uint32_t tim3Hertz = 40000000;
    auto mainDelay = DelayStm32();
    mainDelay.SetupConfiguration({&TIM3->SR, &TIM3->ARR, &TIM3->CR1, &TIM3->PSC, tim3Hertz, TIM_SR_UIF, TIM_CR1_CEN});
#endif

#if defined(USE_DESKTOP)
    auto clk = GPIOOutputDesktop();
    auto clkEn = GPIOOutputDesktop();
    auto clkSelEn = GPIOOutputDesktop();
    auto data = GPIOOutputDesktop();
    auto latch = GPIOOutputDesktop();
    auto ledOE = GPIOOutputDesktop();

    uint32_t outputRegister;
    uint32_t io0Mask = 0x1;
    uint32_t io7Mask = 0x1 << 7;

    {  // IGPIOOutputDesktop setup
        clk.SetupConfiguration({&outputRegister, 0x1 << 0});
        clkEn.SetupConfiguration({&outputRegister, 0x1 << 1});
        clkSelEn.SetupConfiguration({&outputRegister, 0x1 << 2});
        data.SetupConfiguration({&outputRegister, 0x1 << 3});
        latch.SetupConfiguration({&outputRegister, 0x1 << 4});
        ledOE.SetupConfiguration({&outputRegister, 0x1 << 5});
    }

    auto mainDelay = DelayDesktop();
#endif
    HanoverOL037A_GPIOInterface hanoverOL037A_GPIOInterface{&clk, &clkEn, &clkSelEn, &data, &latch, &ledOE};

    auto hanoverOL037A = HanoverOL037A();
    hanoverOL037A.SetGPIOInterface(hanoverOL037A_GPIOInterface);
    hanoverOL037A.SetDelayManager(&mainDelay);

    // Clear the display
    hanoverOL037A.UpdateDisplay();

    const auto resolution = hanoverOL037A.GetResolution();
    while (true) {
        for (uint32_t x = 0; x < resolution.x; ++x) {
            for (uint32_t y = 0; y < resolution.y; ++y) {
                hanoverOL037A.SetPixel({x, y}, 1);
                hanoverOL037A.UpdateDisplay();
                // mainDelay.SynchronousWait_us(100);
            }
        }

        for (uint32_t x = 0; x < resolution.x; ++x) {
            for (uint32_t y = 0; y < resolution.y; ++y) {
                hanoverOL037A.SetPixel({x, y}, 0);
                hanoverOL037A.UpdateDisplay();
                // mainDelay.SynchronousWait_us(100);
            }
        }
    }

    return 0;
}
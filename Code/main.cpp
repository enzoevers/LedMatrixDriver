#include "BitManipulation.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

#if defined(USE_STM32F303XC)
#include "SetupHardwareStm32f303xc.h"
#endif

#if defined(USE_STM32)
#include "GPIOOutputStm32.h"
#endif

#include "HanoverOL037A.h"

int main() {
    HardwareSetup();

#if defined(USE_STM32)
    auto clk = GPIOOutputStm32();
    auto clkSelEn = GPIOOutputStm32();
    auto data = GPIOOutputStm32();
    auto clkEn = GPIOOutputStm32();
    auto latch = GPIOOutputStm32();
    auto ledOE = GPIOOutputStm32();

    { // IGPIOOutputStm32 setup
        clk.SetOutputRegister(&GPIOE->ODR);
        clk.SetPinMask(GPIO_ODR_13);

        clkSelEn.SetOutputRegister(&GPIOE->ODR);
        clkSelEn.SetPinMask(GPIO_ODR_13);

        data.SetOutputRegister(&GPIOE->ODR);
        data.SetPinMask(GPIO_ODR_13);

        clkEn.SetOutputRegister(&GPIOE->ODR);
        clkEn.SetPinMask(GPIO_ODR_13);

        latch.SetOutputRegister(&GPIOE->ODR);
        latch.SetPinMask(GPIO_ODR_13);

        ledOE.SetOutputRegister(&GPIOE->ODR);
        ledOE.SetPinMask(GPIO_ODR_13);
    }
#endif

    HanoverOL037A_GPIOInterface hanoverOL037A_GPIOInterface = {
        clk,
        clkSelEn, 
        data, 
        clkEn, 
        latch, 
        ledOE
    };

    while (true) {
    }

    return 0;
}
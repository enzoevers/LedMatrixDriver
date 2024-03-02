#include "BitManipulation.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

#if defined(USE_STM32F303XC)
#include "SetupHardwareStm32f303xc.h"
#endif

#if defined(USE_STM32)
#include "GPIOOutputStm32.h"
#endif

int main() {
    HardwareSetup();

    // TODO: this currently only works when running ./Scripts/Stm32/Stm32f303xc/BuildStm32f303xc.sh
    // This kind of code should be part of a led matrix specific setup library
    GPIOOutputStm32 pinE13;
    pinE13.SetOutputRegister(&GPIOE->ODR);
    pinE13.SetPinMask(GPIO_ODR_13);

    pinE13.SetState(true);
    // pinE13.SetState(false);

    while (true) {
    }
    return 0;
}
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

    GPIOOutputStm32 pinE13;
    pinE13.SetOutputRegister(&GPIOE->ODR);
    pinE13.SetPinMask(GPIO_ODR_13);

    pinE13.SetState(true);
    // pinE13.SetState(false);

    while (true) {
    }
    return 0;
}
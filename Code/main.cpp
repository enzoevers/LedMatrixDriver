#include "BitManipulation.h"
#include "HardwareSetup.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

//-----

// TODO: Make this available through HardwareSetup.h
#include "stm32f303xc.h"

int main() {
    HardwareSetup();

    SET_BIT(GPIOE->ODR, GPIO_ODR_13);
    // CLEAR_BIT(GPIOE->ODR, GPIO_ODR_13);

    while (true) {
    }
    return 0;
}
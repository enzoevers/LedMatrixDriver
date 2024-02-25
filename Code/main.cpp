#include "Common/BitManipulation.h"
#include "InitHalHardware.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

int main() {
    InitHalHardware();

    SET_BIT(GPIOE->ODR, GPIO_ODR_13);
    // CLEAR_BIT(GPIOE->ODR, GPIO_ODR_13);

    while (true) {
    }
    return 0;
}
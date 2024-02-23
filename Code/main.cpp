#include "Common/BitManipulation.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

void init();

#if defined(USE_STM32F303XC)

#include "stm32f303xc.h"

void SetupClock() {
    __IO uint32_t tmpreg;
    SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN); /* Delay after an RCC peripheral clock enabling */
    tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN);
    UNUSED(tmpreg);
}

void SetupGpio() {
    // Reset
    CLEAR_REG(GPIOE->MODER);
    CLEAR_REG(GPIOE->PUPDR);
    CLEAR_REG(GPIOE->OTYPER);
    CLEAR_REG(GPIOE->OSPEEDR);

    SET_BIT(GPIOE->MODER, 0b1 << GPIO_MODER_MODER13_Pos);
    CLEAR_BIT(GPIOE->MODER, 0b1 << (1 + GPIO_MODER_MODER13_Pos));
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_13);
    CLEAR_BIT(GPIOE->OSPEEDR, 0b11 << GPIO_OSPEEDER_OSPEEDR13_Pos);
    CLEAR_BIT(GPIOE->PUPDR, 0b11 << GPIO_PUPDR_PUPDR13_Pos);
}

void SetupStm32f303xc() {
    SetupClock();
    SetupGpio();
}

void init() { SetupStm32f303xc(); }

#elif defined(USE_DESKTOP)

void SetupDesktop() {}

void init() { SetupDesktop(); }

#endif

int main() {
    init();

    SET_BIT(GPIOE->ODR, GPIO_ODR_13);
    // CLEAR_BIT(GPIOE->ODR, GPIO_ODR_13);

    while (true) {
    }
    return 0;
}
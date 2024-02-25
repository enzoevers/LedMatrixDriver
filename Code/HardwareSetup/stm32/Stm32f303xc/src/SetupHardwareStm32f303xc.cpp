#include "BitManipulation.h"
#include "HardwareSetup.h"
#include "defines.h"
#include "stm32f303xc.h"

//====================
// Local
//====================

static auto SetupClock() -> void {
    __IO uint32_t tmpreg;
    SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN); /* Delay after an RCC peripheral clock enabling */
    tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN);
    UNUSED(tmpreg);
}

static auto SetupGpio() -> void {
    //==========
    // GPIOE
    //==========

    // Reset
    CLEAR_REG(GPIOE->MODER);
    CLEAR_REG(GPIOE->PUPDR);
    CLEAR_REG(GPIOE->OTYPER);
    CLEAR_REG(GPIOE->OSPEEDR);

    // Config IO 13 as output
    SET_BIT(GPIOE->MODER, 0b1 << GPIO_MODER_MODER13_Pos);
    CLEAR_BIT(GPIOE->MODER, 0b1 << (1 + GPIO_MODER_MODER13_Pos));
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_13);
    CLEAR_BIT(GPIOE->OSPEEDR, 0b11 << GPIO_OSPEEDER_OSPEEDR13_Pos);
    CLEAR_BIT(GPIOE->PUPDR, 0b11 << GPIO_PUPDR_PUPDR13_Pos);
}

//====================
// HardwareSetup.h
//====================

auto HardwareSetup() -> void {
    SetupClock();
    SetupGpio();
}

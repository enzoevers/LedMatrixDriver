#include "SetupHardwareStm32f303xc.h"

#include "BitManipulation.h"
#include "defines.h"

static auto SetupClock() -> void;
static auto SetupGpio() -> void;

//====================
// HardwareSetup.h
//====================

auto HardwareSetup() -> void {
    SetupClock();
    SetupGpio();
}

//====================
// Local
//====================

static auto SetupClock() -> void {
    __IO uint32_t tmpreg;
    SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN); /* Delay after an RCC peripheral clock enabling */
    tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN);
    UNUSED(tmpreg);
}

#define CONFIG_OUTPUT(PORT, PIN) { \
    /* Output */ \
    SET_BIT(GPIO##PORT->MODER, 0b1 << GPIO_MODER_MODER##PIN##_Pos); \
    CLEAR_BIT(GPIO##PORT->MODER, 0b1 << (1 + GPIO_MODER_MODER##PIN##_Pos)); \
    \
    /* Push-Pull */ \
    CLEAR_BIT(GPIO##PORT->OTYPER, GPIO_OTYPER_OT_##PIN); \
    \
    /* Low speed */ \
    CLEAR_BIT(GPIO##PORT->OSPEEDR, 0b11 << GPIO_OSPEEDER_OSPEEDR##PIN##_Pos); \
    \
    /* No pull-up, pull-down */ \
    CLEAR_BIT(GPIO##PORT->PUPDR, 0b11 << GPIO_PUPDR_PUPDR##PIN##_Pos); \
}

static auto SetupGpio() -> void {
    // Config port E IO 13
    CONFIG_OUTPUT(E, 13)
}
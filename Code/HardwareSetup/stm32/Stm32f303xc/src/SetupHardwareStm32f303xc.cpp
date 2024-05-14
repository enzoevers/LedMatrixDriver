#include "SetupHardwareStm32f303xc.h"

#include "BitManipulation.h"
#include "defines.h"

static auto SetupClock() -> void;
static auto SetupGpio() -> void;
static auto SetupTimers() -> void;

//====================
// HardwareSetup.h
//====================

auto HardwareSetup() -> void {
    SetupClock();
    SetupGpio();
    SetupTimers();
}

//====================
// Local
//====================

static auto SetupClock() -> void {
    __IO uint32_t tmpreg;

    //----------
    // GPIO clocks
    //----------
    SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIODEN); /* Delay after an RCC peripheral clock enabling */
    tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIODEN);
    UNUSED(tmpreg);

    //----------
    // Timer clocks
    //----------

    // APB1 - HCLK not divided
    RCC->CFGR &= ~RCC_CFGR_PPRE1_2;

    // HLCK - SYSCLK not divided
    RCC->CFGR &= ~RCC_CFGR_HPRE_3;

    //----------
    // PLL
    //----------

    // Stop PLL
    RCC->CR &= ~RCC_CR_PLLON;
    while (RCC->CR & RCC_CR_PLLRDY) {
    }

    // Use HSI/2 as the PLL source clock
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;

    // Max allowed frequency is 72MHz.
    // With a PLL input frequency of 4MHz (HSI/2 = 8MHz / 2 = 4MHz)
    // the max multiplication is 72 / 4 = 18

    // PLL multiplication
    // SYSCLOCK = 4MHz * 10 = 40MHz
    RCC->CFGR |= RCC_CFGR_PLLMUL10;

    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) {
    }

    //----------
    // Main clock
    //----------

    // Select PLL for SYSCLOCK
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Enable HSI clock
    RCC->CR |= RCC_CR_HSION;

    while (!(RCC->CR & RCC_CR_HSIRDY)) {
        // Wait till HSI is ready
    }
}

#define CONFIG_OUTPUT(PORT, PIN)                                                  \
    {                                                                             \
        /* Output */                                                              \
        SET_BIT(GPIO##PORT->MODER, 0b1 << GPIO_MODER_MODER##PIN##_Pos);           \
        CLEAR_BIT(GPIO##PORT->MODER, 0b1 << (1 + GPIO_MODER_MODER##PIN##_Pos));   \
                                                                                  \
        /* Push-Pull */                                                           \
        CLEAR_BIT(GPIO##PORT->OTYPER, GPIO_OTYPER_OT_##PIN);                      \
                                                                                  \
        /* Low speed */                                                           \
        CLEAR_BIT(GPIO##PORT->OSPEEDR, 0b11 << GPIO_OSPEEDER_OSPEEDR##PIN##_Pos); \
                                                                                  \
        /* No pull-up, pull-down */                                               \
        CLEAR_BIT(GPIO##PORT->PUPDR, 0b11 << GPIO_PUPDR_PUPDR##PIN##_Pos);        \
    }

static auto SetupGpio() -> void {
    CONFIG_OUTPUT(D, 10)
    CONFIG_OUTPUT(D, 11)
    CONFIG_OUTPUT(D, 12)
    CONFIG_OUTPUT(D, 13)
    CONFIG_OUTPUT(D, 14)
    CONFIG_OUTPUT(D, 15)
}

static auto SetupTimers() -> void {
    //----------
    // TIM3
    //----------

    // Disable slave mode
    TIM3->SMCR &= ~TIM_SMCR_SMS_0;
    TIM3->SMCR &= ~TIM_SMCR_SMS_1;
    TIM3->SMCR &= ~TIM_SMCR_SMS_2;
    TIM3->SMCR &= ~TIM_SMCR_SMS_3;

    // Set as edge-aligned upcounter
    TIM3->CR1 &= ~TIM_CR1_CMS_0;
    TIM3->CR1 &= ~TIM_CR1_CMS_1;
    TIM3->CR1 &= ~TIM_CR1_DIR;

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}
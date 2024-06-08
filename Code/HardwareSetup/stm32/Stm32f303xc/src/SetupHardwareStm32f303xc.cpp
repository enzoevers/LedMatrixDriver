#include "SetupHardwareStm32f303xc.h"

#include "BitManipulation.h"
#include "defines.h"

static auto SetupClock() -> void;
static auto SetupGpio() -> void;
static auto SetupTimers() -> void;
static auto SetupRTC() -> void;

//====================
// HardwareSetup.h
//====================

auto HardwareSetup() -> void {
    SetupClock();
    SetupGpio();
    SetupTimers();
    SetupRTC();
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

static auto SetupRTC() -> void {
    // Enable the power interface clock
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // Enable write access to the RTC backup registers
    PWR->CR |= PWR_CR_DBP;

    // Reset the RTC clock domain
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;

    // Select LSI for the RTC clock source
    RCC->BDCR &= ~RCC_BDCR_RTCSEL_0;
    RCC->BDCR |= RCC_BDCR_RTCSEL_1;

    // Enable RTC clock
    RCC->BDCR |= RCC_BDCR_RTCEN;

    // Enable LSI at 40KHz
    RCC->CSR |= RCC_CSR_LSION;

    while (!(RCC->CSR & RCC_CSR_LSIRDY))
        ;

    // Unlock RTC
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // Enter initialization mode
    RTC->ISR |= RTC_ISR_INIT;

    // Wait for initialization mode to be entered
    while (!(RTC->ISR & RTC_ISR_INITF))
        ;

    // Set prescaler to go from 40KHz to 1Hz
    // A division of 40.000 is needed which is (PREDIV_A + 1) * (PREDIV_S + 1)
    // where PREDIV_A should be as high as possible to minimize power consumption.
    // The division of 40.000/(PREDIV_A + 1) should be an integer.
    // The division of (40.000/(PREDIV_A + 1))/(PREDIV_S + 1) should be 1.
    //
    // If PREDIV_A is 99 then PREDIV_S must be 399
    // 40.000/(PREDIV_A + 1) = 40.0000 / 100 = 400
    // 400/(PREDIV_S + 1) = 400 / 400 = 1
    const uint32_t PREDIV_A = 99;
    const uint32_t PREDIV_S = 399;
    RTC->PRER = PREDIV_A << RTC_PRER_PREDIV_A_Pos | PREDIV_S << RTC_PRER_PREDIV_S_Pos;

    // Exit initialization mode
    RTC->ISR &= ~RTC_ISR_INIT;

    // Lock RTC
    RTC->WPR = 0;
}
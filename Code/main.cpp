#include "BitManipulation.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

#if defined(USE_STM32F303XC)
#include "SetupHardwareStm32f303xc.h"
#endif

#if defined(USE_STM32)
#include "DateTimeStm32.h"
#include "DelayStm32.h"
#include "GPIOOutputStm32.h"
#endif

#include <cstring>  // std::memset

#include "Fonts/Bitstream_Vera_Sans_Mono/Bitstream_Vera_Sans_Mono_12.h"
#include "Fonts/Bitstream_Vera_Sans_Mono/Bitstream_Vera_Sans_Mono_8.h"
#include "HanoverOL037A.h"
#include "TextBmhFormat.h"

int main() {
    HardwareSetup();

#if defined(USE_STM32)
    auto clk = HAL::STM32::GPIOOutput();
    auto clkEn = HAL::STM32::GPIOOutput();
    auto clkSelEn = HAL::STM32::GPIOOutput();
    auto data = HAL::STM32::GPIOOutput();
    auto latch = HAL::STM32::GPIOOutput();
    auto ledOE = HAL::STM32::GPIOOutput();

    {  // HAL::STM32::GPIOOutput setup
        clk.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_10});
        clkEn.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_11});
        clkSelEn.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_12});
        data.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_13});
        latch.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_14});
        ledOE.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_15});
    }

    uint32_t tim3Hertz = 40000000;
    auto mainDelay = HAL::STM32::Delay();
    mainDelay.SetupConfiguration({&TIM3->SR, &TIM3->ARR, &TIM3->CR1, &TIM3->PSC, tim3Hertz, TIM_SR_UIF, TIM_CR1_CEN});

    auto dateTime = HAL::STM32::DateTime();
    dateTime.SetConfig({.PWR_CR = &PWR->CR,
                        .RTC_WPR = &RTC->WPR,
                        .RTC_ISR = &RTC->ISR,
                        .RTC_CR = &RTC->CR,
                        .RTC_TR = &RTC->TR,
                        .RTC_DR = &RTC->DR,
                        .RTC_SSR = &RTC->SSR,
                        .PWR_CR_DBP_pos = PWR_CR_DBP_Pos,
                        .RTC_ISR_INIT_pos = RTC_ISR_INIT_Pos,
                        .RTC_ISR_INITF_pos = RTC_ISR_INITF_Pos,
                        .RTC_CR_FMF_pos = RTC_CR_FMT_Pos});

#endif

    auto hanoverOL037A = HanoverOL037A();
    HanoverOL037A_GPIOInterface hanoverOL037A_GPIOInterface{&clk, &clkEn, &clkSelEn, &data, &latch, &ledOE};
    hanoverOL037A.SetGPIOInterface(hanoverOL037A_GPIOInterface);
    hanoverOL037A.SetDelayManager(&mainDelay);

    auto textBmhFormat = TextBmhFormat();
    textBmhFormat.SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    std::string text = "Hello World!";
    const auto pixelAreaSize = textBmhFormat.GetRequiredSizeString(text, 1);
    Monochrome pixelAreaData[pixelAreaSize.x * pixelAreaSize.y];
    std::memset(pixelAreaData, 0, sizeof(pixelAreaData));
    PixelArea<Monochrome> pixelArea{pixelAreaSize, pixelAreaData};
    Vec2D offset{0, 0};
    textBmhFormat.SetString(text, 1, offset, pixelArea);

    hanoverOL037A.SetArea({10, 0}, pixelArea);
    hanoverOL037A.UpdateDisplay();

    mainDelay.SynchronousWait_us(1000000);

    // Reset display
    std::memset(pixelAreaData, 0, sizeof(pixelAreaData));
    hanoverOL037A.SetArea({10, 0}, pixelArea);
    hanoverOL037A.UpdateDisplay();

    dateTime.SetDateTime({
        .time = {.hours = 14, .minutes = 4, .seconds = 25, .milliseconds = 0},
        .date = {.year = 2021, .month = 9, .day = 1, .weekday = HAL::Types::Weekday::Wednesday},
    });

    const auto screenResolution = hanoverOL037A.GetResolution();
    HAL::Types::DateTime lastDateTime;
    while (true) {
        auto currentDateTime = dateTime.GetDateTime();
        if (currentDateTime == lastDateTime) {
            continue;
        }
        lastDateTime = currentDateTime;

        char timeTextBuffer[13];
        std::snprintf(&timeTextBuffer[0], sizeof(timeTextBuffer), "%.2u : %.2u : %.2u", currentDateTime.time.hours,
                      currentDateTime.time.minutes, currentDateTime.time.seconds);

        const auto pixelAreaSize = textBmhFormat.GetRequiredSizeString(timeTextBuffer, 1);
        Monochrome pixelAreaData[pixelAreaSize.x * pixelAreaSize.y];
        std::memset(pixelAreaData, 0, sizeof(pixelAreaData));
        PixelArea<Monochrome> pixelArea{pixelAreaSize, pixelAreaData};
        Vec2D offset{0, 0};
        textBmhFormat.SetString(timeTextBuffer, 1, offset, pixelArea);

        hanoverOL037A.SetArea({5, 0}, pixelArea);
        hanoverOL037A.UpdateDisplay();

        // Clear all screen pixels
        for (uint32_t y = 0; y < screenResolution.y; ++y) {
            for (uint32_t x = 0; x < screenResolution.x; ++x) {
                hanoverOL037A.SetPixel({x, y}, 0);
            }
        }
    }

    return 0;
}
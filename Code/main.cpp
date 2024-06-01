#include "BitManipulation.h"
#include "defines.h"  // Generate by cmake based on defines.h.in

#if defined(USE_STM32F303XC)
#include "SetupHardwareStm32f303xc.h"
#endif

#if defined(USE_DESKTOP)
#include "SetupDesktop.h"
#endif

#if defined(USE_STM32)
#include "DelayStm32.h"
#include "GPIOOutputStm32.h"
#endif

#if defined(USE_DESKTOP)
#include <iostream>

#include "DelayDesktop.h"
#include "GPIOOutputDesktop.h"
#endif

#include <cstring>  // std::memset

#include "Fonts/Bitstream_Vera_Sans_Mono/Bitstream_Vera_Sans_Mono_12.h"
#include "Fonts/Bitstream_Vera_Sans_Mono/Bitstream_Vera_Sans_Mono_8.h"
#include "HanoverOL037A.h"
#include "TextBmhFormat.h"

int main() {
    HardwareSetup();

#if defined(USE_STM32)
    auto clk = GPIOOutputStm32();
    auto clkEn = GPIOOutputStm32();
    auto clkSelEn = GPIOOutputStm32();
    auto data = GPIOOutputStm32();
    auto latch = GPIOOutputStm32();
    auto ledOE = GPIOOutputStm32();

    {  // IGPIOOutputStm32 setup
        clk.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_10});
        clkEn.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_11});
        clkSelEn.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_12});
        data.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_13});
        latch.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_14});
        ledOE.SetupConfiguration({&GPIOD->ODR, GPIO_ODR_15});
    }
    uint32_t tim3Hertz = 40000000;
    auto mainDelay = DelayStm32();
    mainDelay.SetupConfiguration({&TIM3->SR, &TIM3->ARR, &TIM3->CR1, &TIM3->PSC, tim3Hertz, TIM_SR_UIF, TIM_CR1_CEN});
#endif

#if defined(USE_DESKTOP)
    auto clk = GPIOOutputDesktop();
    auto clkEn = GPIOOutputDesktop();
    auto clkSelEn = GPIOOutputDesktop();
    auto data = GPIOOutputDesktop();
    auto latch = GPIOOutputDesktop();
    auto ledOE = GPIOOutputDesktop();

    uint32_t outputRegister;
    uint32_t io0Mask = 0x1;
    uint32_t io7Mask = 0x1 << 7;

    {  // IGPIOOutputDesktop setup
        clk.SetupConfiguration({&outputRegister, 0x1 << 0});
        clkEn.SetupConfiguration({&outputRegister, 0x1 << 1});
        clkSelEn.SetupConfiguration({&outputRegister, 0x1 << 2});
        data.SetupConfiguration({&outputRegister, 0x1 << 3});
        latch.SetupConfiguration({&outputRegister, 0x1 << 4});
        ledOE.SetupConfiguration({&outputRegister, 0x1 << 5});
    }

    auto mainDelay = DelayDesktop();
#endif
    auto hanoverOL037A = HanoverOL037A();
    HanoverOL037A_GPIOInterface hanoverOL037A_GPIOInterface{&clk, &clkEn, &clkSelEn, &data, &latch, &ledOE};
    hanoverOL037A.SetGPIOInterface(hanoverOL037A_GPIOInterface);
    hanoverOL037A.SetDelayManager(&mainDelay);

    // Clear the display
    // hanoverOL037A.UpdateDisplay();

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

    // hanoverOL037A.SetPixel({0, 0}, 1);
    // hanoverOL037A.SetPixel({39, 0}, 1);
    // hanoverOL037A.SetPixel({0, 7}, 1);
    // hanoverOL037A.SetPixel({39, 7}, 1);

    // for (auto y = 0; y < 19; y++) {
    //     hanoverOL037A.SetPixel({7, y}, 1);
    // }
    // hanoverOL037A.UpdateDisplay();

    return 0;
}
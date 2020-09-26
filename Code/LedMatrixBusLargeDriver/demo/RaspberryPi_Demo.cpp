#include <ledMatrixDriver/targetDisplays/LedMatrix_Hanover_OL037A.h>
#include <ledMatrixDriver/displayContent/CharacterProvider_AdafruitGFX.h>
#include <ledMatrixDriver/displayController/Controller.h>

#include <unistd.h>
#include <iostream>

int main()
{
    ILedMatrix* display = nullptr;
    ICharacterProvider* characterProvider = nullptr;
    Controller* controller = nullptr;

    const uint8_t clkDestSelectPin = 25;
    const uint8_t clkLedDriverPin = 24;
    const uint8_t dataOutPin = 27;
    const uint8_t clkPin = 23;
    const uint8_t latchLedDriverPin = 22;
    const uint8_t outputEnableLedDriverPin = 18;

#ifdef RASPBERRYPI_PLATFORM
    std::cout << "RASPBERRYPI_PLATFORM is defined\n";
#else
    std::cout << "RASPBERRYPI_PLATFORM is NOT defined\n";
    #endif 

    std::cout << "Creating the display\n";

    display = new LedMatrix_Hanover_OL037A(clkDestSelectPin,
                                            clkLedDriverPin,
                                            dataOutPin,
                                            clkPin,
                                            latchLedDriverPin,
                                            outputEnableLedDriverPin);
    characterProvider = new CharacterProvider_AdafruitGFX();

    std::cout << "Creating the controller\n";

    controller = new Controller(*display, *characterProvider);

    std::cout << "Running the test\n";

    //controller->test();
    //usleep(1000*1000);
    /*for(int i = 0; i < 2; i++) {
        controller->clearDisplay();
        usleep(1000*1000);
        controller->fillDisplay();
        usleep(1000*1000);
    }*/
    controller->clearDisplay();

    controller->showText("2/");
    usleep(1000*1000);

    std::cout << "Cleaning up\n";

    delete(controller);
    controller = nullptr;
    delete(display);
    display = nullptr;
} 
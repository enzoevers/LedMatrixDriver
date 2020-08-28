#include <ledMatrixDriver/targetDisplays/LedMatrix_Hanover_OL037A.h>
#include <ledMatrixDriver/displayController/Controller.h>

#include <iostream>

int main()
{
    ILedMatrix* display = nullptr;
    Controller* controller = nullptr;

    const uint8_t clkDestSelectPin = 4;
    const uint8_t clkLedDriverPin = 17;
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

    std::cout << "Creating the controller\n";

    controller = new Controller(*display);

    std::cout << "Running the test\n";

    controller->test();

    std::cout << "Cleaning up\n";

    delete(controller);
    controller = nullptr;
    delete(display);
    display = nullptr;
}
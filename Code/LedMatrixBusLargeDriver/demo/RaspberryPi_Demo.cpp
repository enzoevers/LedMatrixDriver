#include <ledMatrixDriver/targetDisplays/LedMatrix_Hanover_OL037A.h>
#include <ledMatrixDriver/displayContent/CharacterProvider_AdafruitGFX.h>
#include <ledMatrixDriver/displayController/Controller.h>

#include <ctime> // http://www.cplusplus.com/reference/ctime/strftime/
#include <signal.h>

#include <unistd.h>
#include <iostream>

#define CET (+1)

bool forever = true;

void terminationHandler(int sig) {
    forever = false;
}

int main()
{
    signal(SIGABRT, &terminationHandler);
	signal(SIGTERM, &terminationHandler);
	signal(SIGINT, &terminationHandler);

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

    /*
    std::string spookyText = "~(O.O)~";
    uint16_t spookyTextScrollDelay_ms = 1;
    uint16_t endWidth = controller->getDisplayWidth() - 100;

    for(int i = 0; i < endWidth; i++){
        controller->clearDisplay(false);
        controller->showText(spookyText, i);
        usleep(spookyTextScrollDelay_ms*1000);
    }

    for(int i = endWidth-1; i >= 0; i--){
        controller->clearDisplay(false);
        controller->showText(spookyText, i);
        usleep(spookyTextScrollDelay_ms*1000);
    }
    */

    std::string myText = "Heyhey c:";
    controller->clearDisplay(false);
    controller->showText(myText);
    controller->setBrightness(0x4F00);
 
    usleep(2*1000*1000);

    time_t rawtime;
    int prevMinute = -1;
    struct tm * timeinfo;
    char buffer[20];
    const int checkTimeInterval_s = 2;

    while(forever) {
        time (&rawtime);
        timeinfo = gmtime(&rawtime);

        if(prevMinute != timeinfo->tm_min){
            prevMinute = timeinfo->tm_min;
            timeinfo->tm_hour= (timeinfo->tm_hour+CET)%24;

            strftime (buffer,sizeof(buffer)/sizeof(buffer[0]),"%R %d/%m",timeinfo);
            
            std::string myText(buffer);
            controller->clearDisplay(false);
            controller->showText(myText);
            controller->setBrightness(0x4F00);
        }
        usleep(checkTimeInterval_s*1000*1000);
    }

    std::cout << "Cleaning up\n";

    delete(controller);
    controller = nullptr;
    delete(display);
    display = nullptr;
} 
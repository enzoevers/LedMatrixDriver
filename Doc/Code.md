# Development

## Raspberry pi
### Minimum setup code
Go into the `LedMatrixBusLargeDriver` folder and a folder named `minimumDemo`. In this folder create a file named `RaspberryPi_Demo.cpp` with the following content.

```c
// minimumDemo/RaspberryPi_Demo.cpp

// ledMatrixDriver is provided by the CMakeLists.txt in the driver source
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

    // These are the GPIO pin numbers.
    // NOT the physical pin numbers.
    // They are different!!!
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


    std::cout << "Writing a string to the display\n";

    std::string myText = "Heyhey  c:";
    controller->clearDisplay(false);
    controller->showText(myText, 25, -1);
    controller->setBrightness(0x4F00);
 
    usleep(20*1000*1000);

    std::cout << "Cleaning up\n";

    delete(controller);
    controller = nullptr;
    delete(display);
    display = nullptr;
} 
```
### Compiling
#### cmake
In `LedMatrixBusLargeDriver/CMakeLists.txt` we create a new project and add the required subdirectories

```cmake
# LedMatrixBusLargeDriver/CMakeLists.txt

cmake_minimum_required(VERSION 3.13)

project(LedMatrixDriver)

# Adafruit fonts
add_subdirectory(./lib)

# Display drivers
add_subdirectory(./src)

# Custom application
add_subdirectory(./minimumDemo)
```

In `LedMatrixBusLargeDriver/minimumDemo/CMakeLists.txt`

```cmake
# LedMatrixBusLargeDriver/minimumDemo/CMakeLists.txt

add_executable(ledMatrixDriver_demo_RaspberryPi RaspberryPi_Demo.cpp)

# Needed to make the display driver use the correct implementation
target_compile_definitions(ledMatrixDriver_demo_RaspberryPi PRIVATE RASPBERRYPI_PLATFORM)

# Link against wiringPi
find_library(WIRINGPI_LIBRARIES wiringPi)
target_link_libraries(ledMatrixDriver_demo_RaspberryPi ${WIRINGPI_LIBRARIES})

target_link_libraries(ledMatrixDriver_demo_RaspberryPi ledMatrixDriver)
```

#### Transfering the code to the raspberry pi
**It is assumed that a headless Raspberry Pi is used and that an ssh connection was esteblished.**

From the host pc:

```
cd <path to>LedMatrixBusLargeDriver/../

scp -r LedMatrixBusLargeDriver/* pi@192.168.x.x:~/Documents/LedMatrixBusLargeDriver
```

#### Running cmake
**This is done on the Raspberry Pi!**

RASPBERRYPI_PLATFORM is used in `Platforms.h` to select the correct implementation. It is also used in the `CMakeLists.txt` files.
```sh
$ cd ~/Documents/LedMatrixBusLargeDriver
$ cmake -DRASPBERRYPI_PLATFORM:BOOL=1 && make
```

### Running
Now run:
```sh
$ cd minimumDemo
$ ./ledMatrixDriver_demo_RaspberryPi
```

# Usefull links used during development
### Systemd
- https://www.raspberrypi.org/documentation/linux/usage/systemd.md
- https://raspberrypi.stackexchange.com/questions/54922/systemd-unit-dependencies-to-start-wiringpi-gpio-application
- https://stackoverflow.com/questions/45776003/fixing-a-systemd-service-203-exec-failure-no-such-file-or-directory
- https://askubuntu.com/questions/676007/how-do-i-make-my-systemd-service-run-via-specific-user-and-start-on-boot
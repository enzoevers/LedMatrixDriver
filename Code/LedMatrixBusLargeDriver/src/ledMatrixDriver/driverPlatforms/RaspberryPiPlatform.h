#pragma once

#include <wiringPi.h>
#include <ledMatrixDriver/driverPlatforms/Platforms.h>

class RaspberryPiIo {
    public:
        inline static void setDirection(int pin, GpioDirection direction)
        {
            static bool didWiringPiSetup = false;
            if(didWiringPiSetup == false) {
                //pwmSetRange(0xFF);
                //pwmSetMode(PWM_MODE_BAL);
                wiringPiSetupGpio(); // Allows for the Broadcom pin numbers to be used: http://wiringpi.com/reference/setup/
                didWiringPiSetup = true;
            }

            if(direction == GpioDirection::DIR_OUTPUT) {
                pinMode(pin, OUTPUT);
            } else if(direction == GpioDirection::DIR_INPUT) {
                pinMode(pin, INPUT);
            } else if(direction == GpioDirection::DIR_PWM_OUTPUT) {
                pinMode(pin, PWM_OUTPUT);
            }
        }

        inline static void writeDigital(int pin, bool value)
        {
            digitalWrite(pin, value);
        }

        inline static bool readDigital(int pin)
        {
            return digitalRead(pin);
        }

        inline static void writePwm(int pin, uint16_t value)
        {
            uint16_t val = map(value, 0x0, 0xFFFF, 0x0, 0x3FF);
            pwmWrite(pin, int(val));
        }

        inline static void writeAnalog(int pin, int value)
        {
            pwmWrite (pin, value);
        }

        inline static int readAnalog(int pin)
        {
            return analogRead(pin);
        }

};

class RaspberryPiTiming {
    public:
        inline static void waitMilliseconds(unsigned int time)
        {
            delay(time);
        }

        inline static void waitMicroseconds(unsigned int time)
        {
            delayMicroseconds(time);
        }
};
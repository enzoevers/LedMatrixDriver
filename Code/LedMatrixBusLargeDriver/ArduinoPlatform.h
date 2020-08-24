#pragma once

#include <Arduino.h>

/////////////// TODO recursive include
#include "Platforms.h"

/*namespace PlatformIo 
{

static void setDirection(int pin, GpioDirection direction)
        {
            if(direction == GpioDirection::DIR_OUTPUT || direction == GpioDirection::DIR_PWM_OUTPUT) {
                pinMode(pin, OUTPUT);
            } else if(direction == GpioDirection::DIR_INPUT) {
                pinMode(pin, INPUT);
            }
        }

        static void writeDigital(int pin, bool value)
        {
            digitalWrite(pin, value);
        }

        static bool readDigital(int pin)
        {
            return digitalRead(pin);
        }

        static void writePwm(int pin, uint16_t value)
        {
            uint16_t val = map(value, 0x0, 0xFFFF, 0x0, 0xFF);
            analogWrite(pin, val);
        }

        static void writeAnalog(int pin, int value)
        {
            analogWrite(pin, value);
        }

        static int readAnalog(int pin)
        {
            return analogRead(pin);
        }

}

namespace PlatformTiming 
{

static void delayMilliseconds(unsigned int time)
        {
            delay(time);
        }

        static void delayMicroseconds(unsigned int time)
        {
            ::delayMicroseconds(time); // :: -> Make sure that the global delayMicroseconds (that from Arduino.h) is used
        }
}*/


class ArduinoIo {
    public:
        inline static void setDirection(int pin, GpioDirection direction)
        {
            if(direction == GpioDirection::DIR_OUTPUT || direction == GpioDirection::DIR_PWM_OUTPUT) {
                pinMode(pin, OUTPUT);
            } else if(direction == GpioDirection::DIR_INPUT) {
                pinMode(pin, INPUT);
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
            uint16_t val = map(value, 0x0, 0xFFFF, 0x0, 0xFF);
            analogWrite(pin, val);
        }

        inline static void writeAnalog(int pin, int value)
        {
            analogWrite(pin, value);
        }

        inline static int readAnalog(int pin)
        {
            return analogRead(pin);
        }

};

class ArduinoTiming {
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
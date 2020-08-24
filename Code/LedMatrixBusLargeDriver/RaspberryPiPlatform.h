#pragma once

#include <wiringPi.h>

/*
namespace GpioHandling 
{

void setDirection(int pin, GpioDirection direction)
{
    if(direction == GpioDirection::DIR_OUTPUT) {
        pinMode(pin, OUTPUT);
    } else if(direction == GpioDirection::DIR_INPUT) {
        pinMode(pin, INPUT);
    } else if(direction == GpioDirection::DIR_PWM_OUTPUT) {
        pinMode(pin, PWM_OUTPUT);
    }
}

void writeDigital(int pin, bool value)
{
    digitalWrite(pin, value);
}

bool readDigital(int pin)
{
    return digitalRead(pin);
}

void writePwm(int pin, uint16_t value)
{
    uint8_t val = GpioHandling::map(value, 0, (2^16)-1, 0, (2^10)-1);
    pwmWrite(pin, val);
}

void writeAnalog(int pin, int value)
{
    analogWrite(pin, value);
}

int readAnalog(int pin)
{
    return analogRead(pin);
}

}

namespace Timing
{

}
*/

class RaspberryPiIo {
    public:
        inline static void setDirection(int pin, GpioDirection direction)
        {
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
#pragma once

namespace PlatformAbstraction 
{
    enum class GpioDirection {
        DIR_OUTPUT,
        DIR_INPUT,
        DIR_PWM_OUTPUT
    };
    
    // Copied from the Arduino map() reference: https://www.arduino.cc/reference/en/language/functions/math/map/
    static long map(long x, long in_min, long in_max, long out_min, long out_max) {
        long outRange = out_max - out_min;
        long inRange = in_max - in_min;
        long offsetIn = x - in_min;

        return offsetIn * outRange / inRange + out_min;
    }

template <class T>
class PlatformIo {
    public:
        static void setDirection(int pin, GpioDirection direction)
        {
            T::setDirection(pin, direction);
        }

        static void writeDigital(int pin, bool value)
        {
            T::writeDigital(pin, value);
        }

        static bool readDigital(int pin)
        {
            return T::readDigital(pin);
        }

        static void writePwm(int pin, uint16_t value)
        {
            T::writePwm(pin, value);
        }

        static void writeAnalog(int pin, int value)
        {
            T::writeAnalog(pin, value);
        }

        static int readAnalog(int pin)
        {
            return T::readAnalog(pin);
        }
};

template <class T>
class PlatformTiming {
    public:
        static void waitMilliseconds(unsigned int time)
        {
            T::waitMilliseconds(time);
        }

        static void waitMicroseconds(unsigned int time)
        {
            T::waitMicroseconds(time);
        }
};

#ifdef ARDUINO_PLATFORM
    #include "ArduinoPlatform.h"
    static const PlatformIo<ArduinoIo> gpioHandling;
    static const PlatformTiming<ArduinoTiming> timingHandling;
#elif RASPBERRYPI_PLATFORM
    #include "RaspberryPiPlatform.h"
    static const PlatformIo<RaspberryPiIo> gpioHandling;
    static const PlatformTiming<RaspberryPiTiming> timingHandling;
#endif

}
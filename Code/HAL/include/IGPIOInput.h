#pragma once

namespace HAL {
/*!
 * \class IGPIOInput
 * \brief This interface is used to configure and read a single GPIO input pin.
 */
class IGPIOInput {
   public:
    virtual ~IGPIOInput() = default;

    // \note When this functions is not able to get the state of the GPIO, false is returned.
    virtual auto GetState() -> bool = 0;
};

}  // namespace HAL
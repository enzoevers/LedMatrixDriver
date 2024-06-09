#pragma once

namespace HAL {
/*!
 * \class IGPIOOutput
 * \brief This interface is used to configure and control a single GPIO output pin.
 */
class IGPIOOutput {
   public:
    virtual ~IGPIOOutput() = default;

    // \note When this functions is not able to set the state of the GPIO, returns false. Otherwise returns true
    virtual auto SetState(bool on) -> bool = 0;

    // \note When this functions is not able to get the state of the GPIO, false is returned.
    virtual auto GetState() -> bool = 0;
};

}  // namespace HAL
#pragma once

/*!
 * \class IGPIOOutput
 * \brief This interface is used to configure and control a single GPIO output pin.
 */
class IGPIOOutput {
   public:
    virtual ~IGPIOOutput() = default;

    virtual auto SetState(bool on) -> void = 0;
    virtual auto GetState() -> bool = 0;
};
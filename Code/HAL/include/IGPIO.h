#pragma once

/*!
 * \class IGPIOOutput
 * \brief This interface is used to configure and control a single GPIO output pin.
 */
class IGPIOOutput {
    virtual ~IGPIOOutput() = default;

    virtual auto Init() -> bool = 0;

    virtual auto SetState(bool on) -> void = 0;
    virtual auto GetState() -> bool = 0;
};
#pragma once

#include "States/States.h"

namespace Clock {

/*!
 @brief Interface to define a state of the state machine.
*/
class IState {
   public:
    virtual ~IState() = default;

    virtual auto GetStateName() const -> const States = 0;

    virtual auto HasSubState() const -> bool = 0;

    /*!
        @brief Get the sub state if it exists.
        @return The sub state if it exists, otherwise States::None.
    */
    virtual auto GetSubState() const -> const States = 0;

    /*!
     @brief Enter the state. Must be called only once.
    */
    virtual auto Enter() -> void = 0;

    /*!
     @brief Update the state. Must be called once every loop.
    */
    virtual auto Update() -> void = 0;

    /*!
     @brief Exit the state. Must be called only once.
    */
    virtual auto Exit() -> void = 0;

    /*!
     @brief Check if the state should transition to another state.
     @return States::None if should not transition, otherwise the state to transition to.
    */
    virtual auto ShouldTransitionToState() -> States = 0;
};

}  // namespace Clock

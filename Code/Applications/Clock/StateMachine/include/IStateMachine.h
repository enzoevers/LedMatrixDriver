#pragma once

#include "IState.h"

namespace Clock {

/*!
 @brief Interface to define the main state machine that handles the internal state transitions.
*/
class IStateMachine {
   public:
    virtual ~IStateMachine() = default;

    /*!
        @brief Start the state machine at the initial state.
               This initial state is defined by the implementation.
    */
    virtual auto Start() -> void = 0;

    /*!
        @brief To be called once every iteratoin
    */
    virtual auto Update() -> void = 0;

    /*!
        @brief Get the current state.
    */
    virtual auto GetCurrentState() const -> const IState* = 0;
};

}  // namespace Clock
#pragma once

namespace Clock {

enum class States {
    /*!
     @brief The initial state of the clock before the state machine has
            been started.
    */
    None,

    /*!
     @brief The main application of showing the running clock.
            This state is also responsible to reading the input buttons
            so change from ShowingClock to SettingClock.
    */
    ShowingClock,

    /*!
     @brief Changing the current time that the clock shows.
    */
    SettingClock,

    //=========================
    // Substates of SettingClock
    //=========================

    /*!
     @brief The state to change the hour of the clock.
    */
    SettingHour,

    /*!
     @brief The state to change the minutes of the clock.
    */
    SettingMinute,

    /*!
     @brief The state to change the second of the clock.
    */
    SettingSecond

};

}  // namespace Clock
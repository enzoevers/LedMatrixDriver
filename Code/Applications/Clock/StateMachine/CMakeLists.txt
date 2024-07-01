add_library(StateMachine STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/StateMachine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/States/ShowingClock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/States/SettingClock.cpp
)

target_include_directories(StateMachine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(StateMachine
    Common
    HAL
    Displays
    TextProvider
)

if(TEST_ON_PC)
    add_subdirectory(test)
endif()


add_library(hal-desktop STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIOOutputDesktop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DelayDesktop.cpp
)

target_include_directories(hal-desktop PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
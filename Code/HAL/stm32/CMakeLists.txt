add_library(hal-stm32 STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIOOutputStm32.cpp
)

target_include_directories(hal-stm32 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
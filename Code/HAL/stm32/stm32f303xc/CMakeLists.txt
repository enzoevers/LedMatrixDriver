add_library(hal-stm32f303xc STATIC
    GPIOOutputStm32F303xc.cpp
)

# Generic HAL includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

# Specific HAL includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(hal-stm32f303xc
    CMSIS::STM32::F303VC
    STM32::NoSys
)
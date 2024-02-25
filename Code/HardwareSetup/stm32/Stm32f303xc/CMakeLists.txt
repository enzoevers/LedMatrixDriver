add_library(hardware-init-stm32f303xc STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/InitHardwareStm32f303xc.cpp
)

target_link_libraries(hardware-init-stm32f303xc
    STM32::NoSys
    CMSIS::STM32::F303VC
    Common
)
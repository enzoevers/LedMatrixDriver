add_library(hardware-setup-stm32f303xc STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SetupHardwareStm32f303xc.cpp
)

target_include_directories(hardware-setup-stm32f303xc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hardware-setup-stm32f303xc PUBLIC
    STM32::NoSys
    CMSIS::STM32::F303VC
)

target_link_libraries(hardware-setup-stm32f303xc PRIVATE
    Common
)
add_library(hal-stm32 STATIC
    
)


target_link_libraries(hal-stm32
    CMSIS::STM32::F303VC
    STM32::NoSys
)
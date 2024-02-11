add_library(DisplayControl STATIC
    DisplayCommunication.cpp
)

target_link_libraries(DisplayControl
    CMSIS::STM32::F303VC
    STM32::NoSys
)
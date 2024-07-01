add_subdirectory(StateMachine)

if(NOT TEST_ON_PC)
    add_executable(${APPLICATION_NAME} main.cpp)

    target_link_libraries(${APPLICATION_NAME}
        Common
        HAL
        Displays
        TextProvider
        StateMachine
    )

    if(USE_STM32)
        add_subdirectory(HardwareSetup)
        
        stm32_print_size_of_target(${APPLICATION_NAME})
        stm32_generate_binary_file(${APPLICATION_NAME})
        stm32_generate_hex_file(${APPLICATION_NAME})
    endif()
endif()
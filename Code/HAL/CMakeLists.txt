include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(TEST_ON_PC OR USE_STM32)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stm32)
endif()

if(TEST_ON_PC OR USE_ARDUINO_HAL)
endif()

if(TEST_ON_PC)
    add_subdirectory(test)
endif()
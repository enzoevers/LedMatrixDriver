file(WRITE ${CMAKE_BINARY_DIR}/empty.cpp "")
add_library(HAL STATIC ${CMAKE_BINARY_DIR}/empty.cpp)

if(TEST_ON_PC OR USE_STM32)
    add_subdirectory(stm32)
    target_include_directories(hal-stm32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    if(NOT TEST_ON_PC)
        if(USE_STM32F303XC)
            target_link_libraries(hal-stm32 hardware-setup-stm32f303xc)
        endif()
    endif()
    
    target_link_libraries(HAL hal-stm32)
endif()

if(TEST_ON_PC)
    add_subdirectory(test)
endif()
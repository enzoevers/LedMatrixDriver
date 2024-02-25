include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(USE_STM32)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stm32)
endif()
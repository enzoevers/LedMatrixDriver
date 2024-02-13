cmake_minimum_required(VERSION 3.25)

if(USE_STM32)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)
endif()

if(TEST_ON_PC)
    include(GoogleTest)
endif()

set(PROJECT_NAME LedDisplayDriverProject)
set(EXECUTABLE_NAME LedDisplayDriverExecutable)

project(${PROJECT_NAME} C CXX ASM)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Doc)

set(MAIN_SOURCE_FILES main.cpp)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

add_subdirectory(${PROJECT_SOURCE_DIR}/HAL)

add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE_FILES})

if(USE_STM32)
    target_link_libraries(${EXECUTABLE_NAME}
        hal-stm32
        STM32::NoSys
    )
    stm32_print_size_of_target(${EXECUTABLE_NAME})
    stm32_generate_binary_file(${EXECUTABLE_NAME})
    stm32_generate_hex_file(${EXECUTABLE_NAME})
endif()

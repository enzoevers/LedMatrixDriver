cmake_minimum_required(VERSION 3.25)

if(USE_STM32_HAL)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)
endif()

set(PROJECT_NAME LedDisplayDriverProject)
set(EXECUTABLE_NAME LedDisplayDriverExecutable)

project(${PROJECT_NAME} C CXX ASM)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Doc)

set(MAIN_SOURCE_FILES main.cpp)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

if(USE_STM32_HAL)
    stm32_fetch_cmsis(F3)
    set(CMSIS_COMP_LIST "")
    list(APPEND CMSIS_COMP_LIST STM32F3)
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/DisplayControl)

if(USE_STM32_HAL)
    add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE_FILES})
    target_link_libraries(${EXECUTABLE_NAME}
        DisplayControl
        CMSIS::STM32::F303VC
        STM32::NoSys
    )
    stm32_print_size_of_target(${EXECUTABLE_NAME})
    stm32_generate_binary_file(${EXECUTABLE_NAME})
    stm32_generate_hex_file(${EXECUTABLE_NAME})
endif()

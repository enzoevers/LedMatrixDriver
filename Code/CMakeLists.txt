cmake_minimum_required(VERSION 3.25)

configure_file(defines.h.in defines.h)

if(USE_STM32)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)
    set(CMSIS_COMP_LIST "")
endif()

set(PROJECT_NAME LedDisplayDriverProject)
project(${PROJECT_NAME} C CXX ASM)

set(MAIN_SOURCE_FILES main.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Any F3 chip
if(USE_STM32F3)
    stm32_fetch_cmsis(F3)
    list(APPEND CMSIS_COMP_LIST "STM32F3")
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
endif()

if(TEST_ON_PC OR USE_DESKTOP)
    set(CMAKE_CXX_STANDARD 20)
endif()

if(TEST_ON_PC)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(fuzztest)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
endif()

if(GENERATE_DOCS)
    add_subdirectory(Doc)
else()
    if(NOT TEST_ON_PC)
        add_subdirectory(HardwareSetup)
    endif()
    
    add_subdirectory(Common)
    add_subdirectory(HAL)
    add_subdirectory(Displays)
    
    if(NOT TEST_ON_PC)
        set(EXECUTABLE_NAME LedDisplayDriverExecutable)
        add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE_FILES})

        target_link_libraries(${EXECUTABLE_NAME}
            Common
            HAL
            Displays
        )

        if(USE_STM32)    
            stm32_print_size_of_target(${EXECUTABLE_NAME})
            stm32_generate_binary_file(${EXECUTABLE_NAME})
            stm32_generate_hex_file(${EXECUTABLE_NAME})
        endif()
    endif()
endif()

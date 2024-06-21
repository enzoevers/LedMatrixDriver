cmake_minimum_required(VERSION 3.25)

configure_file(defines.h.in defines.h)

include(${CMAKE_CURRENT_SOURCE_DIR}/Libs/HAL/pre_project.cmake)

set(PROJECT_NAME LedDisplayDriverProject)
project(${PROJECT_NAME} C CXX ASM)

if(GENERATE_DOCS)
    add_subdirectory(Doc)
    return()
endif()

set(MAIN_SOURCE_FILES main.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(TEST_ON_PC)
    set(CMAKE_CXX_STANDARD 20)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../External/fuzztest ${CMAKE_CURRENT_BINARY_DIR}/fuzztest)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
endif()

add_subdirectory(Libs)

if(NOT TEST_ON_PC)
    add_subdirectory(HardwareSetup)
endif()

if(NOT TEST_ON_PC)
    set(EXECUTABLE_NAME LedDisplayDriverExecutable)
    add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE_FILES})
     target_link_libraries(${EXECUTABLE_NAME}
        Common
        HAL
        Displays
        TextProvider
    )
     if(USE_STM32)
        stm32_print_size_of_target(${EXECUTABLE_NAME})
        stm32_generate_binary_file(${EXECUTABLE_NAME})
        stm32_generate_hex_file(${EXECUTABLE_NAME})
    endif()
endif()

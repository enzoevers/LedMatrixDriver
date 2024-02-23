cmake_minimum_required(VERSION 3.25)

configure_file(defines.h.in defines.h)

if(USE_STM32)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)
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
endif()

set(PROJECT_NAME LedDisplayDriverProject)
set(EXECUTABLE_NAME LedDisplayDriverExecutable)

project(${PROJECT_NAME} C CXX ASM)

if(GENERATE_DOCS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Doc)
endif()

if(TEST_ON_PC)
    add_subdirectory(${CMAKE_SOURCE_DIR}/fuzztest)
endif()

if(TEST_ON_PC OR USE_DESKTOP)
    set(CMAKE_CXX_STANDARD 23)
endif()

set(MAIN_SOURCE_FILES main.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(NOT GENERATE_DOCS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/HAL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/Common)
endif()

add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME}
    Common
)

if(USE_STM32)
    target_link_libraries(${EXECUTABLE_NAME}
        hal-stm32
    )

    stm32_print_size_of_target(${EXECUTABLE_NAME})
    stm32_generate_binary_file(${EXECUTABLE_NAME})
    stm32_generate_hex_file(${EXECUTABLE_NAME})
endif()

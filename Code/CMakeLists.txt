cmake_minimum_required(VERSION 3.25)

configure_file(defines.h.in defines.h)

if(USE_STM32)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)
    set(CMSIS_COMP_LIST "")
endif()

set(PROJECT_NAME LedDisplayDriverProject)
set(EXECUTABLE_NAME LedDisplayDriverExecutable)

project(${PROJECT_NAME} C CXX ASM)

set(MAIN_SOURCE_FILES main.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

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

# Any F3 chip
if(USE_STM32F3)
    stm32_fetch_cmsis(F3)
    list(APPEND CMSIS_COMP_LIST "STM32F3")
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
endif()

if(GENERATE_DOCS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Doc)
endif()

if(TEST_ON_PC)
    add_subdirectory(${CMAKE_SOURCE_DIR}/fuzztest)
endif()

if(TEST_ON_PC OR USE_DESKTOP)
    set(CMAKE_CXX_STANDARD 23)
endif()

if(NOT GENERATE_DOCS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/Common)

    add_subdirectory(${PROJECT_SOURCE_DIR}/HAL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/Displays)

    target_include_directories(Displays INTERFACE
        ${PROJECT_SOURCE_DIR}/HAL/include
    )

    if(TEST_ON_PC)
        target_include_directories(Displays INTERFACE
            ${PROJECT_SOURCE_DIR}/HAL/test/mocks
        )
    endif()
    
    add_subdirectory(${PROJECT_SOURCE_DIR}/HardwareSetup)
endif()

if(NOT TEST_ON_PC)
    add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE_FILES})

    target_link_libraries(${EXECUTABLE_NAME}
        Common
    )

   

    if(USE_STM32)    
        if(USE_STM32F303XC)
            target_link_libraries(${EXECUTABLE_NAME}
                hardware-setup-stm32f303xc
            )
            target_link_libraries(hal-stm32
                hardware-setup-stm32f303xc
            )

            if(HANOVER_OL037A)
                target_link_libraries(Hanover-OL037A
                    hardware-setup-stm32f303xc
                )
            endif()
        endif()

        target_link_libraries(${EXECUTABLE_NAME}
            hal-stm32
        )

        stm32_print_size_of_target(${EXECUTABLE_NAME})
        stm32_generate_binary_file(${EXECUTABLE_NAME})
        stm32_generate_hex_file(${EXECUTABLE_NAME})
    endif()

    if(USE_DESKTOP)
        target_link_libraries(${EXECUTABLE_NAME}
            hardware-setup-desktop
        )
        
        target_link_libraries(${EXECUTABLE_NAME}
            hal-desktop
        )
    endif()

    if(HANOVER_OL037A)
        target_link_libraries(${EXECUTABLE_NAME}
            Hanover-OL037A
        )
    endif()
endif()

add_library(hal-stm32 STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIOOutputStm32.cpp
)

target_include_directories(hal-stm32 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(USE_STM32)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/stm32-cmake/cmake/")
    include(stm32/common)
    include(stm32/devices)

    set(CMSIS_COMP_LIST "")
    list(APPEND CMSIS_COMP_LIST)
endif()

# Any F3 chip or when testing
if(USE_STM32F301X8 OR USE_STM32F303XC)
    stm32_fetch_cmsis(F3)
    list(APPEND STM32F3)
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
endif()

if(USE_STM32F303XC)
    add_subdirectory(stm32f303xc)
endif()
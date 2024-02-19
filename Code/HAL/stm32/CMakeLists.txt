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
    
    target_link_libraries(hal-stm32
        STM32::NoSys
    )
endif()

# Any F3 chip or when testing
if(USE_STM32F303XC)
    stm32_fetch_cmsis(F3)
    list(APPEND CMSIS_COMP_LIST "STM32F3")
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)

    target_link_libraries(hal-stm32
        CMSIS::STM32::F303VC
    )
endif()
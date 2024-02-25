if(USE_STM32)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/stm32-cmake/cmake/")
    include(stm32/common)
    include(stm32/devices)

    set(CMSIS_COMP_LIST "")
endif()

# Any F3 chip
if(USE_STM32F3)
    stm32_fetch_cmsis(F3)
    list(APPEND CMSIS_COMP_LIST "STM32F3")
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED) 
endif()

if(USE_STM32F303XC)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Stm32f303xc)
endif()
# Any F3 chip
if(USE_STM32F3)
    stm32_fetch_cmsis(F3)
    list(APPEND CMSIS_COMP_LIST "STM32F3")
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
endif()

if(USE_STM32F303XC)
    add_subdirectory(Stm32f303xc)
endif()
if(USE_STM32 AND (NOT GENERATE_DOCS))
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../External/stm32-cmake/cmake/stm32_gcc.cmake)
    set(CMSIS_COMP_LIST "")
endif()
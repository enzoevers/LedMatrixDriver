if(USE_STM32)
    add_subdirectory(stm32)

    if(USE_STM32F303XC)
        target_include_directories(hardware-setup-stm32f303xc PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )
    endif()
endif()

if(USE_DESKTOP)
    add_subdirectory(desktop)
    target_include_directories(hardware-setup-desktop PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif()

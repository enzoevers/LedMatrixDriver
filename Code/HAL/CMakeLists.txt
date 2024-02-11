add_library(hal STATIC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(USE_STM32_HAL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stm32)
    target_link_libraries(hal PRIVATE 
        hal-stm32
    )
endif()

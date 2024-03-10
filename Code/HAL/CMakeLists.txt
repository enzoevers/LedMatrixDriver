if(TEST_ON_PC OR USE_STM32)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stm32)
    target_link_libraries(hal-stm32
        Common
    )
    target_include_directories(hal-stm32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

if(TEST_ON_PC OR USE_DESKTOP)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/desktop)
    target_link_libraries(hal-desktop
        Common
    )
    target_include_directories(hal-desktop PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

if(TEST_ON_PC)
    add_subdirectory(test)
endif()
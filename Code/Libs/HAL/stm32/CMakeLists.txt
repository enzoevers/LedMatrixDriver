add_library(hal-stm32 STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIOOutputStm32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIOInputStm32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DelayStm32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DateTimeStm32.cpp
)

target_include_directories(hal-stm32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(hal-stm32 Common)
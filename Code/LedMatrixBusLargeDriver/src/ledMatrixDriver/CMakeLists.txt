add_subdirectory(./displayContent/Images)

add_library(ledMatrixDriver ${CMAKE_CURRENT_SOURCE_DIR}/displayController/Controller.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/displayContent/CharacterProvider_AdafruitGFX.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/displayContent/ImageProvider.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/targetDisplays/LedMatrix_Hanover_OL037A.cpp)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
target_include_directories(ledMatrixDriver PUBLIC ${PARENT_DIR})

target_link_libraries(ledMatrixDriver Adafruit_GFX)

if(DEFINED RASPBERRYPI_PLATFORM)
    target_compile_definitions(ledMatrixDriver PRIVATE RASPBERRYPI_PLATFORM)
elseif(DEFINED ARDUINO_PLATFORM)
    target_compile_definitions(ledMatrixDriver PRIVATE ARDUINO_PLATFORM)
endif()
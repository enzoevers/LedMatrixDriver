add_library(TextProvider STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TextBmhFormat.cpp
)

set(FONT_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/Generated/Fonts")
set(GENERATE_FONT_SCRIPT "${PROJECT_SOURCE_DIR}/../Scripts/Fonts/GenerateFonts.sh")

execute_process(COMMAND mkdir -p ${FONT_INCLUDE_DIR})
execute_process(COMMAND ${GENERATE_FONT_SCRIPT} ${FONT_INCLUDE_DIR} "Bitstream Vera Sans Mono" 8)
execute_process(COMMAND ${GENERATE_FONT_SCRIPT} ${FONT_INCLUDE_DIR} "Bitstream Vera Sans Mono" 12)
execute_process(COMMAND ${GENERATE_FONT_SCRIPT} ${FONT_INCLUDE_DIR} "Bitstream Vera Sans Mono" 16)

target_include_directories(TextProvider PUBLIC ${FONT_INCLUDE_DIR}/../)

target_include_directories(TextProvider PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TextProvider Displays)

if(TEST_ON_PC)
    add_subdirectory(test)
endif()
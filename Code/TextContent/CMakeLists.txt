add_library(TextContent STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TextContent.cpp
)

target_include_directories(TextContent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TextContent Displays)

if(TEST_ON_PC)
    add_subdirectory(test)
endif()
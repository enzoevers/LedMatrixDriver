file(WRITE ${CMAKE_BINARY_DIR}/empty.cpp "")
add_library(Displays STATIC ${CMAKE_BINARY_DIR}/empty.cpp)

if(TEST_ON_PC OR HANOVER_OL037A)
    add_subdirectory(HanoverOL037A)
    target_include_directories(Hanover-OL037A PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(Displays Hanover-OL037A)
endif()

if(TEST_ON_PC)
    target_link_libraries(Displays TestHAL_mocks)
    add_subdirectory(test)
endif()
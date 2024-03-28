add_library(Displays INTERFACE)

if(TEST_ON_PC OR HANOVER_OL037A)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/HanoverOL037A)
    target_include_directories(Hanover-OL037A PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(Hanover-OL037A Displays)
endif()

if(TEST_ON_PC)
    add_subdirectory(test)
endif()
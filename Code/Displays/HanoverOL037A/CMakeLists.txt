add_library(Hanover-OL037A STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HanoverOL037A.cpp
)

target_include_directories(Hanover-OL037A PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
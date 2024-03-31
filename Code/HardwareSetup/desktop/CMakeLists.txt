add_library(hardware-setup-desktop STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SetupDesktop.cpp
)

target_include_directories(hardware-setup-desktop PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
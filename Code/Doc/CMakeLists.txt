find_package(Doxygen REQUIRED doxygen)

if (DOXYGEN_FOUND)
    doxygen_add_docs(doxygen
                        ${PROJECT_SOURCE_DIR}/main.cpp
                        ${PROJECT_SOURCE_DIR}/HardwareSetup
                        ${PROJECT_SOURCE_DIR}/Common
                        ${PROJECT_SOURCE_DIR}/HAL
                        ${PROJECT_SOURCE_DIR}/Displays
                        COMMENT "Generate man pages"
                    )

    set(doxyfile_in ${CMAKE_CURRENT_BINARY_DIR}/build/Doxyfile.doxygen)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
endif()
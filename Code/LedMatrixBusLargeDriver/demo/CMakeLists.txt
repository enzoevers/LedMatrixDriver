if(DEFINED RASPBERRYPI_PLATFORM)
    set(platform RaspberryPi)   
    add_executable(ledMatrixDriver_demo_${platform} RaspberryPi_Demo.cpp)
    target_compile_definitions(ledMatrixDriver_demo_${platform} PRIVATE RASPBERRYPI_PLATFORM)
    # Link against wiringPi
    find_library(WIRINGPI_LIBRARIES wiringPi)
    target_link_libraries(ledMatrixDriver_demo_${platform} ${WIRINGPI_LIBRARIES})
elseif(DEFINED ARDUINO_PLATFORM)
    set(platform Arduino)   
    add_executable()
    target_compile_definitions(ledMatrixDriver_demo_${platform} PRIVATE ARDUINO_PLATFORM)
endif()

target_link_libraries(ledMatrixDriver_demo_${platform} ledMatrixDriver)
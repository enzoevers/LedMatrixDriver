add_executable(TestHAL
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalStm32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalDesktop.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalIGPIOOutput.cpp
)

link_fuzztest(TestHAL)

target_link_libraries(TestHAL PRIVATE 
  gtest
  gtest_main  
  hal-stm32
  hal-desktop
)

gtest_discover_tests(TestHAL)
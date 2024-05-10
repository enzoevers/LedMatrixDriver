add_executable(TestHAL
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalIGPIOOutput.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalGPIOOutputStm32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalGPIOOutputDesktop.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalDelayStm32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalDelayDesktop.cpp
)

link_fuzztest(TestHAL)

target_include_directories(TestHAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

target_link_libraries(TestHAL PRIVATE 
  gtest
  gmock
  gtest_main
  hal-stm32
  hal-desktop
)

gtest_discover_tests(TestHAL)
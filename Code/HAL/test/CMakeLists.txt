file(WRITE ${CMAKE_BINARY_DIR}/empty.cpp "")
add_library(TestHAL_mocks STATIC ${CMAKE_BINARY_DIR}/empty.cpp)
target_include_directories(TestHAL_mocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

add_executable(TestHAL
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalIGPIOOutput.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalGPIOOutputStm32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalDelayStm32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalDateTimeStm32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalGPIOOutputDesktop.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHalDelayDesktop.cpp
)

link_fuzztest(TestHAL)

target_link_libraries(TestHAL PRIVATE 
  gtest
  gmock
  gtest_main
  TestHAL_mocks
  HAL
)

gtest_discover_tests(TestHAL)
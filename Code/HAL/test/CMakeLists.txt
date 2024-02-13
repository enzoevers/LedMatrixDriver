include(GoogleTest)

add_executable(TestHAL test_hal_stm32.cpp)
target_link_libraries(TestHAL PRIVATE 
  hal-stm32
  gtest
  gtest_main
)

gtest_discover_tests(TestHAL)
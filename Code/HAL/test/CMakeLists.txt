add_executable(TestHAL
  test_hal_stm32.cpp
  test_hal_desktop.cpp
)

link_fuzztest(TestHAL)

target_link_libraries(TestHAL PRIVATE 
  gtest
  gtest_main  
  hal-stm32
  hal-desktop
)

gtest_discover_tests(TestHAL)
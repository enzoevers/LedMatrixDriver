add_executable(TestHAL
  test_hal_stm32.cpp
  test_hal_desktop.cpp
)

target_link_libraries(TestHAL PRIVATE 
  hal-stm32
  hal-desktop
  gtest
  gtest_main
)

gtest_discover_tests(TestHAL)
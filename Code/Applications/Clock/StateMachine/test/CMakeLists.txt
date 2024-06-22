add_executable(TestStateMachine
  ${CMAKE_CURRENT_SOURCE_DIR}/TestStateMachine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestShowingClock.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestSettingClock.cpp
)

link_fuzztest(TestStateMachine)

target_link_libraries(TestStateMachine PRIVATE 
  gtest
  gmock
  gtest_main
  TestHAL_mocks
  TestDisplays_mocks
  TestTextProvider_mocks
  StateMachine
)

gtest_discover_tests(TestStateMachine)
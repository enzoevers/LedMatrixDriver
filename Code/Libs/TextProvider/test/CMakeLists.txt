add_executable(TestTextProvider
  ${CMAKE_CURRENT_SOURCE_DIR}/TestTextBmhFormat.cpp
)

link_fuzztest(TestTextProvider)

target_link_libraries(TestTextProvider PRIVATE
  gtest
  gmock
  gtest_main
  TextProvider
)

gtest_discover_tests(TestTextProvider)
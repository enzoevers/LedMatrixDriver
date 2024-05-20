add_executable(TestTextContent
  ${CMAKE_CURRENT_SOURCE_DIR}/TestTextContent.cpp
)

link_fuzztest(TestTextContent)

target_link_libraries(TestTextContent PRIVATE
  gtest
  gmock
  gtest_main
  TextContent
)

gtest_discover_tests(TestTextContent)
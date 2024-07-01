file(WRITE ${CMAKE_BINARY_DIR}/empty.cpp "")
add_library(TestTextProvider_mocks STATIC ${CMAKE_BINARY_DIR}/empty.cpp)
target_include_directories(TestTextProvider_mocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

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
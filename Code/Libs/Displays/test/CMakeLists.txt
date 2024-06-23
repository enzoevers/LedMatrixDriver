file(WRITE ${CMAKE_BINARY_DIR}/empty.cpp "")
add_library(TestDisplays_mocks STATIC ${CMAKE_BINARY_DIR}/empty.cpp)
target_include_directories(TestDisplays_mocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

add_executable(TestDisplays
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHanoverOL037A.cpp
)

link_fuzztest(TestDisplays)

target_link_libraries(TestDisplays PRIVATE 
  gtest
  gmock
  gtest_main  
  Displays
)

gtest_discover_tests(TestDisplays)
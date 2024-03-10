add_executable(TestDisplays
  ${CMAKE_CURRENT_SOURCE_DIR}/TestIDisplayMonochrome.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestHanoverOL037A.cpp
)

link_fuzztest(TestDisplays)

target_link_libraries(TestDisplays PRIVATE 
  gtest
  gmock
  gtest_main  
  Hanover-OL037A
)

gtest_discover_tests(TestDisplays)
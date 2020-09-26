# http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html
# https://mariuszbartosik.com/directory-structure-for-a-c-project/

cmake_minimum_required(VERSION 3.13)

project(LedMatrixDriver)

add_subdirectory(./lib)
add_subdirectory(./src)
add_subdirectory(./demo)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_base64 test_base64.cpp lib/base64.cpp)
add_test(NAME base64 COMMAND test_base64)
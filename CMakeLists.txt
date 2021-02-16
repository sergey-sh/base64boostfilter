cmake_minimum_required(VERSION 3.10)
project(base64)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65 COMPONENTS iostreams)

add_executable(base64 main.cpp base64.hpp)
target_link_libraries(base64 ${Boost_LIBRARIES})

enable_testing()
#if(BUILD_TESTING)
add_subdirectory(test)
#endif()
add_subdirectory(example)


cmake_minimum_required(VERSION 3.10)

add_executable(example_gzip gziptostring.cpp ../base64.hpp)
target_link_libraries(example_gzip ${Boost_LIBRARIES})

add_executable(example_gunzip gunzipfromstring.cpp ../base64.hpp)
target_link_libraries(example_gunzip ${Boost_LIBRARIES})


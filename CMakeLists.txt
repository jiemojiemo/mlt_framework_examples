cmake_minimum_required(VERSION 3.10)

project(mlt_examples)

add_subdirectory(mlt ${CMAKE_BINARY_DIR}/mlt)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(src)
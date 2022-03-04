cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 99)

project(Bricks)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(Bricks)

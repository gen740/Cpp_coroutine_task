cmake_minimum_required(VERSION 3.20)

project(test CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS "on")

message("
============================================================
Building on ${CMAKE_BUILD_TYPE} Mode
============================================================")

# Set cmpile flags
add_compile_options(-std=c++20 -O3)
add_compile_options(-Wall)

file(GLOB SOURCE src/*.cpp)

add_executable(main ${SOURCE})

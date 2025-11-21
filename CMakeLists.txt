cmake_minimum_required(VERSION 3.10.0)
project(osu-parser VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

add_executable(osu-parser Tests.cpp)
target_include_directories(osu-parser PRIVATE include)
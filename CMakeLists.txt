cmake_minimum_required(VERSION 3.3)
project(testProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(testProject ${SOURCE_FILES})
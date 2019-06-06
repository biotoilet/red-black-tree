cmake_minimum_required(VERSION 3.5)

project(learn)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-Wall -g -DDEBUG)

set(src "main.cpp")

add_executable(${PROJECT_NAME} ${src})

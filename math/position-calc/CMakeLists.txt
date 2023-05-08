cmake_minimum_required(VERSION 3.14)

project(position-calc C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(position-calc STATIC)

target_sources(position-calc PUBLIC
"${PROJECT_SOURCE_DIR}/include/position-calc.h"
"${PROJECT_SOURCE_DIR}/src/position-calc.c"
)
target_include_directories(position-calc PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)

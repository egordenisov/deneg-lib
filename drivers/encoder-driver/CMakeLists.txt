cmake_minimum_required(VERSION 3.14)

project(encoder-driver C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(encoder-driver STATIC)

target_sources(encoder-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include/encoder-driver.h"
"${PROJECT_SOURCE_DIR}/src/encoder-driver.c"
)
target_include_directories(encoder-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)


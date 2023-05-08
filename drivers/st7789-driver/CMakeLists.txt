cmake_minimum_required(VERSION 3.14)

project(st7789-driver C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(st7789-driver STATIC)

target_sources(st7789-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include/st7789-driver.h"
"${PROJECT_SOURCE_DIR}/src/st7789-driver.c"
)
target_include_directories(st7789-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)

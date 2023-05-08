cmake_minimum_required(VERSION 3.14)

project(button-driver C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(button-driver STATIC)

target_sources(button-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include/button-driver.h"
"${PROJECT_SOURCE_DIR}/src/button-driver.c"
)
target_include_directories(button-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)
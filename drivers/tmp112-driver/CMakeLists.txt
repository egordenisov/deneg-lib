cmake_minimum_required(VERSION 3.14)

project(tmp112-driver C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(tmp112-driver STATIC)

target_sources(tmp112-driver PUBLIC
"${PROJECT_SOURCE_DIR}/src/tmp112-driver.c"

"${PROJECT_SOURCE_DIR}/include/tmp112-driver.h"
"${PROJECT_SOURCE_DIR}/include/tmp112-regs.h"
)
target_include_directories(tmp112-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)

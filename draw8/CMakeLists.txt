cmake_minimum_required(VERSION 3.14)

project(draw8 C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(draw8 STATIC)

target_sources(draw8 PUBLIC
"${PROJECT_SOURCE_DIR}/include/draw8.h"
"${PROJECT_SOURCE_DIR}/src/draw8.c"
"${PROJECT_SOURCE_DIR}/src/text.c"
"${PROJECT_SOURCE_DIR}/src/geom.c"
)
target_include_directories(draw8 PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)
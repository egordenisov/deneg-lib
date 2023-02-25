cmake_minimum_required(VERSION 3.14)

project(st7789-driver)

add_library(${PROJECT_NAME})

add_subdirectory(tests)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include/st7789-driver.h"
    "${PROJECT_SOURCE_DIR}/src/st7789-driver.c"
)
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

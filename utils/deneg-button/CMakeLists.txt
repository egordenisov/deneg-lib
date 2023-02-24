cmake_minimum_required(VERSION 3.14)

project(deneg-button)

add_library(${PROJECT_NAME})

add_subdirectory(tests)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include/deneg-button.h"
    "${PROJECT_SOURCE_DIR}/src/deneg-button.c"
)

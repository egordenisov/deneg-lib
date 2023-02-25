cmake_minimum_required(VERSION 3.14)

project(deneg-button)
set(CMAKE_CXX_STANDARD 14)

add_library(deneg-button STATIC)

target_sources(deneg-button PUBLIC
"${PROJECT_SOURCE_DIR}/include/deneg-button.h"
"${PROJECT_SOURCE_DIR}/src/deneg-button.c"
)
target_include_directories(deneg-button PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)

#
# GoogleTest
#

enable_testing()
add_executable(deneg-button-tests 
    "tests/test_deneg-button.cpp"
)
target_link_libraries(deneg-button-tests PUBLIC
    deneg-button
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(deneg-button-tests)

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

#
# GoogleTest
#
if(DEFINED BUILD_WITH_TESTS)
enable_testing()
add_executable(position-calc-tests 
    "tests/test-position-calc.cpp"
)
target_link_libraries(position-calc-tests PUBLIC
    position-calc
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(position-calc-tests)
endif()
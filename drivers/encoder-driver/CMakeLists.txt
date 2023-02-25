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

#
# GoogleTest
#

enable_testing()
add_executable(encoder-driver-tests 
    "tests/test-encoder-driver.cpp"
)
target_link_libraries(encoder-driver-tests PUBLIC
    encoder-driver
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(encoder-driver-tests)

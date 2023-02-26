cmake_minimum_required(VERSION 3.14)

project(encoder-driver C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(st7789-driver STATIC)

target_sources(st7789-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include/st7789-driver.h"
"${PROJECT_SOURCE_DIR}/src/st7789-driver.c"
)
target_include_directories(st7789-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)

#
# GoogleTest
#

enable_testing()
add_executable(st7789-driver-tests 
    "tests/test-st7789-driver.cpp"
)
target_link_libraries(st7789-driver-tests PUBLIC
    st7789-driver
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(st7789-driver-tests)
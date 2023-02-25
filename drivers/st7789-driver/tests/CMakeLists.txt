cmake_minimum_required(VERSION 3.14)
project(st7789-tests)
add_executable(${PROJECT_NAME} test_st7789-driver.cpp)
set(CMAKE_CXX_STANDARD 11)

target_link_libraries(${PROJECT_NAME} PUBLIC
    st7789-driver
    gtest
    gtest_main
)
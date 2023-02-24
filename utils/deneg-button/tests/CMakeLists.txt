cmake_minimum_required(VERSION 3.14)
project(tests)
add_executable(${PROJECT_NAME} test_deneg-button.cpp)
set(CMAKE_CXX_STANDARD 11)

target_link_libraries(${PROJECT_NAME} PUBLIC
    deneg-button
    gtest
    gtest_main
)
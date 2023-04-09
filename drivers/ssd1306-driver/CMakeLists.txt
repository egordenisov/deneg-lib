cmake_minimum_required(VERSION 3.14)

project(ssd1306-driver C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(ssd1306-driver STATIC)

target_sources(ssd1306-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include/ssd1306-driver.h"
"${PROJECT_SOURCE_DIR}/include/ssd1306-fonts.h"
"${PROJECT_SOURCE_DIR}/src/ssd1306-driver.c"
)
target_include_directories(ssd1306-driver PUBLIC
"${PROJECT_SOURCE_DIR}/include"
)


# GoogleTest

# if(DEFINED BUILD_WITH_TESTS)
# enable_testing()
# add_executable(ssd1306-driver-tests 
#     "tests/test-ssd1306-driver.cpp"
# )
# target_link_libraries(ssd1306-driver-tests PUBLIC
#     ssd1306-driver
#     GTest::gtest_main
# )
# include(GoogleTest)
# gtest_discover_tests(ssd1306-driver-tests)
# endif()

#
# GoogleTest
#
if(DEFINED BUILD_WITH_TESTS)
enable_testing()
add_executable(driver-tests 
    "${PROJECT_SOURCE_DIR}/tests/drivers/test-button-driver.cpp"
    "${PROJECT_SOURCE_DIR}/tests/drivers/test-encoder-driver.cpp"
    "${PROJECT_SOURCE_DIR}/tests/drivers/test-tmp112-driver.cpp"
)

target_link_libraries(driver-tests PUBLIC
    button-driver
    encoder-driver
    tmp112-driver
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(driver-tests)
endif()

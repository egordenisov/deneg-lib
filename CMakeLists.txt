cmake_minimum_required(VERSION 3.14)

project(deneg)

if(DEFINED BUILD_WITH_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )
  FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(drivers/button-driver)
add_subdirectory(drivers/st7789-driver)
add_subdirectory(drivers/encoder-driver)
add_subdirectory(drivers/ssd1306-driver)

add_subdirectory(math/position-calc)

if(DEFINED BUILD_WITH_TESTS)
  enable_testing()
  include(GoogleTest)
endif()


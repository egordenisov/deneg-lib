cmake_minimum_required(VERSION 3.14)

project(deneg)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(utils/deneg-button)

add_subdirectory(drivers/st7789-driver)


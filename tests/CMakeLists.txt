cmake_minimum_required(VERSION 3.16.3)

project(tests)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(string-test main.cpp string_utils.test.cpp)

target_include_directories(
string-test
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
string-test INTERFACE
  String::Utilities
)

target_link_libraries(string-test PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
enable_testing()

include(Catch)
catch_discover_tests(string-test)

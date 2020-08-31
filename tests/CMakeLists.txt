cmake_minimum_required(VERSION 3.16.3)

project(tests)

find_package(Catch2 REQUIRED)

add_executable(string-test main.cpp string_utils.test.cpp)

target_include_directories(
string-test
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
string-test
  String::Utilities
)

include(CTest)
enable_testing()

include(Catch)
catch_discover_tests(string-test)

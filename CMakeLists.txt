cmake_minimum_required(VERSION 3.16.3)
project(string-utility LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(out main.cpp src/string_utils.hpp)
#target_compile_features(out PUBLIC cxx_std_17)

target_link_libraries(out String::Utilities)


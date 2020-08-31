cmake_minimum_required(VERSION 3.16.3)

project(Utilities 
	VERSION 1.0.0
	DESCRIPTION "String Utilitiy library")


add_library(strutils INTERFACE)
target_include_directories(strutils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(strutils INTERFACE cxx_std_17)

add_library(String::Utilities ALIAS strutils)

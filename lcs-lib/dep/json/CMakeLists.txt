cmake_minimum_required(VERSION 3.1)

project(json)
add_library(json INTERFACE)
target_include_directories(json INTERFACE src/)

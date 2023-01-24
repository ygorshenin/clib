cmake_minimum_required(VERSION 2.8.12.2)

find_package(gflags REQUIRED)

project(langford CXX)

clib_add_executable(langford main.cc)
target_link_libraries(langford gflags)

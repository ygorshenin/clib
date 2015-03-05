cmake_minimum_required(VERSION 3.1)

project(clib C CXX)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(PROJECT_BINARY_DIR bin)
set(PROJECT_SOURCE_DIR src)

add_compile_options(
  "-Wall"
  "-pedantic"
  "-std=c++11"
  )

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${GTEST_INCLUDE_DIRS}
  )

add_subdirectory(${PROJECT_SOURCE_DIR})

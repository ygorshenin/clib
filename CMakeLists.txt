cmake_minimum_required(VERSION 2.8.12.2)

project(clib C CXX)

option(USE_ASAN "Build application with clang Address Sanitizer" OFF)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_compile_options(
  "-Wall"
  "-pedantic"
  "-std=c++20"
)

if (USE_ASAN)
  message("Address Sanitizer is enabled.")
  add_compile_options(
    "-fsanitize=address"
    "-fno-omit-frame-pointer"
    )
endif()

include_directories(
  ${GTEST_INCLUDE_DIRS}
)

function(clib_add_executable executable)
  add_executable(${executable} ${ARGN})
  if (USE_ASAN)
    target_link_libraries(${executable} "-fsanitize=address")
  endif()
endfunction()

function(clib_add_unittest executable)
  clib_add_executable(${executable} ${ARGN})
  target_link_libraries(${executable}
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
  )
endfunction()

function(clib_add_library library)
  add_library(${library} STATIC ${ARGN})
endfunction()

add_subdirectory(algo)
add_subdirectory(benchmarks)

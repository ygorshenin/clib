cmake_minimum_required(VERSION 2.8.12.2)

project(clib C CXX)

option(USE_CLANG "Build application with clang" OFF)
option(USE_ASAN "Build application with clang Address Sanitizer" OFF)

if (USE_CLANG)
  message("Using clang compiler")
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
endif()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(PROJECT_SOURCE_DIR src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_compile_options(
  "-Wall"
  "-pedantic"
  "-std=c++11"
  )

if (USE_ASAN)
  message("Address Sanitizer is enabled.")
  add_compile_options(
    "-fsanitize=address"
    "-fno-omit-frame-pointer"
    )
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}
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
  add_library(${library} SHARED ${ARGN})
endfunction()

add_subdirectory(${PROJECT_SOURCE_DIR})

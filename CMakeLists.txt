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

function(CLibAddExecutable executable sources)
  add_executable(${executable} ${sources})
  target_link_libraries(${executable}
    ${CMAKE_THREAD_LIBS_INIT}
  )
endfunction()

function(CLibAddTest executable sources)
  CLibAddExecutable(${executable} ${sources})
  target_link_libraries(${executable}
    ${GTEST_BOTH_LIBRARIES}
  )
endfunction()

function(CLibAddLibrary library sources)
  add_library(${library} SHARED ${sources})
endfunction()

add_subdirectory(${PROJECT_SOURCE_DIR})

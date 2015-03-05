set(SOURCES
  suffix_array.cc
  suffix_array.h
  )

add_library(algo SHARED ${SOURCES})

set(TEST-SOURCES
  suffix_array_unittest.cc
  )

add_executable(algo_unittest ${TEST-SOURCES})
target_link_libraries(algo_unittest
  algo
  ${GTEST_BOTH_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  )

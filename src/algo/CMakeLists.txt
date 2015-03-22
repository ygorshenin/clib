clib_add_library(algo
  sequences/fenwick.h
  sequences/rmq.h
  strings/kmp.cc
  strings/kmp.h
  strings/suffix_array.cc
  strings/suffix_array.h
  )

clib_add_unittest(algo_unittest
  sequences/fenwick_unittest.cc
  sequences/rmq_unittest.cc
  strings/kmp_unittest.cc
  strings/suffix_array_unittest.cc
  )

target_link_libraries(algo_unittest algo)

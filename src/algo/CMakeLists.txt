clib_add_library(algo
  fenwick.h
  rmq.h
  suffix_array.cc
  suffix_array.h
  )

clib_add_unittest(algo_unittest
  fenwick_unittest.cc
  rmq_unittest.cc
  suffix_array_unittest.cc
  )

target_link_libraries(algo_unittest algo)

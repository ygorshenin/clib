clib_add_library(algo
  numeric/matrix.h
  sequences/fenwick.h
  sequences/rmq.h
  sequences/segtree.h
  sequences/treap.h
  strings/kmp.cc
  strings/kmp.h
  strings/suffix_array.cc
  strings/suffix_array.h
  )

clib_add_unittest(algo_unittest
  numeric/matrix_unittest.cc
  sequences/fenwick_unittest.cc
  sequences/rmq_unittest.cc
  sequences/segtree_unittest.cc
  sequences/treap_unittest.cc
  strings/kmp_unittest.cc
  strings/suffix_array_unittest.cc
  )

target_link_libraries(algo_unittest algo)

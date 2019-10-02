include_directories(BEFORE .)

clib_add_library(algo
  bits/bit_vector.h
  bits/bits.h
  bits/rs_table.cc
  bits/rs_table.h
  graph/kuhn.h
  math/math.cc
  numeric/fft.cc
  numeric/matrix.cc
  numeric/simplex.cc
  sequences/fenwick.h
  sequences/dsu.cc
  sequences/heap.h
  sequences/median.h
  sequences/merge_sort.h
  sequences/rmq.h
  sequences/sat2.cc
  sequences/segtree.h
  sequences/treap.h
  strings/aho_corasick.cc
  strings/kmp.cc
  strings/suffix_array.cc
)

clib_add_unittest(algo_unittest
  bits/bit_vector_unittest.cc
  bits/bits_unittest.cc
  bits/rs_table_unittest.cc
  graph/kuhn_unittest.cc
  numeric/fft_unittest.cc
  numeric/matrix_unittest.cc
  numeric/simplex_unittest.cc
  sequences/dsu_unittest.cc
  sequences/fenwick_unittest.cc
  sequences/heap_unittest.cc
  sequences/median_unittest.cc
  sequences/merge_sort_unittest.cc
  sequences/rmq_unittest.cc
  sequences/sat2_unittest.cc
  sequences/segtree_unittest.cc
  sequences/treap_unittest.cc
  strings/aho_corasick_unittest.cc
  strings/kmp_unittest.cc
  strings/suffix_array_unittest.cc
)

target_link_libraries(algo_unittest algo)

set(SOURCES
  suffix_array.cc
  suffix_array.h
  )

set(TEST-SOURCES
  suffix_array_unittest.cc
  )

CLibAddLibrary(algo "${SOURCES}")

CLibAddTest(algo_unittest "${TEST-SOURCES}")
target_link_libraries(algo_unittest algo)

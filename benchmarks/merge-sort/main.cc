#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "sequences/merge_sort.h"

int main() {
  srand(0);

  size_t const size = 512 * 1000000;
  std::vector<int> data(size);
  for (size_t i = 0; i < size; ++i)
    data[i] = rand();

  algo::MergeSort(data.size(), data.data(), algo::SortOptions(6144 * 1024, 64));

  if (!std::is_sorted(data.begin(), data.end())) {
    fprintf(stderr, "Merge sort error\n");
    return 1;
  }
  return 0;
}

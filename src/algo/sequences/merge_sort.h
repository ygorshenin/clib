#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "algo/sequences/heap.h"

namespace algo {
// This is a wrapper around raw memory buffer. Used to hold start
// address and number of elements.
template <typename T> struct Buffer {
  Buffer() : data_(nullptr), size_(0) {}
  Buffer(T *data, size_t size) : data_(data), size_(size) {}

  const T *data_;
  const size_t size_;
};

// This class is used to pass options to an external memory merge-sort
// algorithm.
struct SortOptions {
  SortOptions()
      : cache_size_(6144 * 1024 /* L3 cache size for i7-4710MQ */),
        cache_line_size_(64 /* cache line size for i7-4710MQ */) {}

  SortOptions(size_t cache_size, size_t cache_line_size)
      : cache_size_(cache_size), cache_line_size_(cache_line_size) {}

  const size_t cache_size_;
  const size_t cache_line_size_;
};

// Merges |num_buffers| buffers to out, returns a pointer to the end
// of the sorted sequence.
template <typename T>
T *Merge(size_t num_buffers, Buffer<T> buffers[], T *out) {
  using Value = std::pair<T /* elem */, size_t /* buffer */>;
  Heap<Value> heap;

  std::vector<size_t> offsets(num_buffers, 0);
  for (size_t i = 0; i < num_buffers; ++i) {
    if (buffers[i].size_ != 0)
      heap.push(std::make_pair(buffers[i].data_[0], i));
  }

  while (!heap.empty()) {
    auto &elem = heap.min();

    *out++ = elem.first;
    const size_t buffer = elem.second;
    const size_t offset = ++offsets[buffer];
    if (offset == buffers[buffer].size_) {
      heap.pop();
      continue;
    }
    const T &value = buffers[buffer].data_[offset];
    if (value <= elem.first) {
      elem.first = value;
    } else {
      elem.first = value;
      heap.OnValueIncreased(0);
    }
  }
  return out;
}

// An external-memory merge-sort algorithm. Not quite efficient and
// can be beat by std::sort() or even std::stable_sort() algorithms.
template <typename T>
void MergeSort(size_t n, T *data, const SortOptions &options = SortOptions()) {
  const size_t cache_line_size =
      std::max(options.cache_line_size_, static_cast<size_t>(1));
  const size_t cache_size = std::max(options.cache_size_, cache_line_size);

  // Initial sequence is initially split on runs |run_length| size
  // each.
  const size_t run_length = std::max((cache_size + sizeof(T) - 1) / sizeof(T),
                                     static_cast<size_t>(1));

  // The number of runs to be merged at each Merge() call.
  const size_t runs_to_merge = std::max(
      options.cache_size_ / (2 * cache_line_size), static_cast<size_t>(2));

  std::vector<Buffer<T>> curRuns;
  size_t offset = 0;
  while (offset < n) {
    size_t length = std::min(n - offset, run_length);
    std::sort(data + offset, data + offset + length);

    curRuns.emplace_back(data + offset, length);
    offset += length;
  }

  // In-place merging is a quite complex algorithm, so it's much
  // easier to just create an auxiliary buffer.
  std::vector<T> nxtBuffer(n);
  T *curData = data;
  T *nxtData = nxtBuffer.data();

  while (curRuns.size() > 1) {
    std::vector<Buffer<T>> nxtRuns;

    size_t i = 0;
    T *start = nxtData;
    while (i < curRuns.size()) {
      const size_t num_runs = std::min(runs_to_merge, curRuns.size() - i);
      T *end = Merge(num_runs, &curRuns[i], start);

      nxtRuns.emplace_back(start, end - start);
      start = end;
      i += num_runs;
    }
    curRuns.swap(nxtRuns);
    std::swap(curData, nxtData);
  }

  if (curData == nxtBuffer.data())
    std::copy(nxtBuffer.begin(), nxtBuffer.end(), data);
}
} // namespace algo

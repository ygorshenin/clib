#pragma once

namespace algo {
// This class is a simple and efficient implementation of a classical
// heap data structure. Data stored in the heap should be at least
// movable.
template <typename T>
class Heap {
 public:
  Heap() = default;

  template <typename V>
  void Push(V&& value);
  void Pop();

  const T& Min() const { return buffer_[0]; }
  T& Min() { return buffer_[0]; }

  const T& operator[](size_t i) const { return buffer_[i]; }
  T& operator[](size_t i) { return buffer_[i]; }

  const size_t Size() const { return buffer_.size(); }
  const bool Empty() const { return buffer_.empty(); }

  void OnValueDecreased(size_t pos);
  void OnValueIncreased(size_t pos);

 private:
  static size_t LeftChild(size_t pos) { return 2 * pos + 1; }
  static size_t RightChild(size_t pos) { return 2 * pos + 2; }
  static size_t Parent(size_t pos) { return (pos - 1) / 2; }

  std::vector<T> buffer_;
};

template <typename T>
template <typename V>
void Heap<T>::Push(V&& value) {
  buffer_.push_back(std::forward<V>(value));
  OnValueDecreased(Size() - 1);
}

template <typename T>
void Heap<T>::Pop() {
  std::swap(buffer_.front(), buffer_.back());
  buffer_.pop_back();
  if (!Empty())
    OnValueIncreased(0);
}

template <typename T>
void Heap<T>::OnValueDecreased(size_t pos) {
  while (pos != 0) {
    const auto parent = Parent(pos);
    if (!(buffer_[pos] < buffer_[parent]))
      break;
    std::swap(buffer_[parent], buffer_[pos]);
    pos = parent;
  }
}

template <typename T>
void Heap<T>::OnValueIncreased(size_t pos) {
  const auto left_child = LeftChild(pos);
  const auto right_child = RightChild(pos);

  if (left_child >= buffer_.size())
    return;

  if (right_child >= buffer_.size()) {
    if (buffer_[left_child] < buffer_[pos]) {
      std::swap(buffer_[pos], buffer_[left_child]);
      OnValueIncreased(left_child);
    }
    return;
  }

  if (!(buffer_[left_child] < buffer_[pos]) &&
      !(buffer_[right_child] < buffer_[pos])) {
    return;
  }

  if (!(buffer_[pos] < buffer_[left_child]) &&
      !(buffer_[right_child] < buffer_[left_child])) {
    std::swap(buffer_[pos], buffer_[left_child]);
    OnValueIncreased(left_child);
    return;
  }

  std::swap(buffer_[pos], buffer_[right_child]);
  OnValueIncreased(right_child);
}
}  // namespace algo

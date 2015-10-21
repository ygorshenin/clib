#pragma once

namespace algo {
// This class is a simple and efficient implementation of a classical
// heap data structure. Data stored in the heap should be at least
// movable.
template <typename T> class Heap {
public:
  Heap() = default;

  template <typename V> void push(V &&value);
  void pop();

  inline const T &min() const { return buffer_[0]; }
  inline T &min() { return buffer_[0]; }
  inline const T &operator[](size_t i) const { return buffer_[i]; }
  inline T &operator[](size_t i) { return buffer_[i]; }
  inline const size_t size() const { return buffer_.size(); }
  inline const bool empty() const { return buffer_.empty(); }

  void OnValueDecreased(size_t pos);
  void OnValueIncreased(size_t pos);

private:
  static inline size_t LeftChild(size_t pos) { return 2 * pos + 1; }
  static inline size_t RightChild(size_t pos) { return 2 * pos + 2; }
  static inline size_t Parent(size_t pos) { return (pos - 1) / 2; }

  std::vector<T> buffer_;
};

template <typename T> template <typename V> void Heap<T>::push(V &&value) {
  buffer_.push_back(std::forward<V>(value));
  OnValueDecreased(size() - 1);
}

template <typename T> void Heap<T>::pop() {
  std::swap(buffer_.front(), buffer_.back());
  buffer_.pop_back();
  if (!empty())
    OnValueIncreased(0);
}

template <typename T> void Heap<T>::OnValueDecreased(size_t pos) {
  T value = std::move(buffer_[pos]);
  while (pos != 0) {
    const size_t parent = Parent(pos);
    if (!(value < buffer_[parent]))
      break;
    buffer_[pos] = std::move(buffer_[parent]);
    pos = parent;
  }
  buffer_[pos] = std::move(value);
}

template <typename T> void Heap<T>::OnValueIncreased(size_t pos) {
  T value = std::move(buffer_[pos]);

  while (true) {
    const size_t left_child = LeftChild(pos);
    const size_t right_child = RightChild(pos);

    if (left_child >= buffer_.size())
      break;

    if (right_child >= buffer_.size()) {
      if (buffer_[left_child] < value) {
        buffer_[pos] = std::move(buffer_[left_child]);
        pos = left_child;
      }
      break;
    }

    size_t min =
        buffer_[left_child] < buffer_[right_child] ? left_child : right_child;

    if (!(buffer_[min] < value))
      break;
    buffer_[pos] = std::move(buffer_[min]);
    pos = min;
  }

  buffer_[pos] = std::move(value);
}
} // namespace algo

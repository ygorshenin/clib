#pragma once

#include <vector>

namespace algo {
template <typename Graph>
class Kuhn {
public:
  Kuhn(const Graph& graph, int from, int to, std::vector<int>& match_to)
      : graph_(graph), match_to_(match_to), from_(from), to_(to) {}

  void Go() {
    int n = graph_.NumVertices();
    match_to_.assign(n, -1);
    colors_.assign(n, -1);

    std::vector<int> unmatched;
    unmatched.reserve(to_ - from_);
    GreedyMatch(unmatched);

    for (int color = 0; !unmatched.empty(); ++color) {
      bool ok = false;

      int j = 0;
      for (int i = 0; i < static_cast<int>(unmatched.size()); ++i) {
        const int u = unmatched[i];
        if (Match(u, color))
          ok = true;
        else
          unmatched[j++] = u;
      }
      unmatched.resize(j);

      if (!ok)
        break;
    }
  }

private:
  void GreedyMatch(std::vector<int>& unmatched) {
    for (int u = from_; u < to_; ++u) {
      bool matched = false;
      for (const auto& edge : graph_.OutEdges(u)) {
        int v = edge.Target();
        if (match_to_[v] < 0) {
          match_to_[u] = v;
          match_to_[v] = u;
          matched = true;
          break;
        }
      }
      if (!matched)
        unmatched.push_back(u);
    }
  }

  bool Match(int u, int color) {
    if (colors_[u] == color)
      return false;
    colors_[u] = color;
    for (const auto& edge : graph_.OutEdges(u)) {
      const int v = edge.Target();
      const int w = match_to_[v];
      if (w < 0 || Match(w, color)) {
        match_to_[u] = v;
        match_to_[v] = u;
        return true;
      }
    }
    return false;
  }

  const Graph& graph_;
  std::vector<int>& match_to_;
  std::vector<int> colors_;
  int from_;
  int to_;
};
}  // namespace algo

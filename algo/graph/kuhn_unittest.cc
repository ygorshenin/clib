#include <gtest/gtest.h>

#include <vector>

#include "graph/kuhn.h"

namespace {
struct Edge {
  Edge(int to) : to_(to) {}

  inline int Target() const { return to_; }

  int to_;
};

template <typename Edge>
struct Graph {
  Graph(int n) : adj_(n) {}

  inline int NumVertices() const { return adj_.size(); }
  inline const std::vector<Edge>& OutEdges(int u) const { return adj_[u]; }

  std::vector<std::vector<Edge>> adj_;
};
}  // namespace

namespace algo {
TEST(Kuhn, Simple) {
  // Test taken from http://www.spoj.com/problems/MATCHING/
  const int ls = 5;
  const int rs = 4;

  std::vector<std::pair<int, int>> edges = {{5, 2}, {1, 2}, {4, 3}, {3, 1}, {2, 2}, {4, 4}};

  Graph<Edge> graph(ls + rs);
  for (const auto& edge : edges)
    graph.adj_[edge.first - 1].push_back(ls + edge.second - 1);

  std::vector<int> match_to;
  Kuhn<decltype(graph)> kuhn(graph, 0, ls, match_to);
  kuhn.Go();

  ASSERT_EQ(ls + rs, match_to.size());

  int matched = 0;
  for (int u = 0; u < static_cast<int>(match_to.size()); ++u) {
    if (match_to[u] < 0)
      continue;
    if (u < ls)
      ++matched;
    int v = match_to[u];
    if (u < ls) {
      ASSERT_GE(v, ls);
      ASSERT_LT(v, ls + rs);
    } else {
      ASSERT_GE(v, 0);
      ASSERT_LT(v, ls);
    }
    ASSERT_EQ(u, match_to[v]);
  }

  ASSERT_EQ(3, matched);
}
}  // namespace algo

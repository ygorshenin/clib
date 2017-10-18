#include <gtest/gtest.h>

#include <vector>

#include "algo/sequences/sat2.h"

using namespace algo;
using namespace std;

namespace {
TEST(SAT2, Smoke) {
  using X = DNF::X;

  {
    SAT2 sat2(DNF{});
    ASSERT_FALSE(sat2.Unsat());
    ASSERT_TRUE(sat2.Assignment().empty());
  }

  {
    SAT2 sat2(DNF{{X(0), X(1), X(2)}});
    ASSERT_FALSE(sat2.Unsat());
    ASSERT_EQ(sat2.Assignment(), vector<bool>({true, true, true}));
  }

  {
    SAT2 sat2(DNF{{!X(0), !X(1), X(2)}});
    ASSERT_FALSE(sat2.Unsat());
    ASSERT_EQ(sat2.Assignment(), vector<bool>({false, false, true}));
  }

  {
    SAT2 sat2(DNF{
        {X(0) | X(1), X(1) | X(2), X(2) | X(3), !X(0) | !X(2), !X(1) | !X(3)}});
    ASSERT_FALSE(sat2.Unsat());
    ASSERT_EQ(sat2.Assignment(), vector<bool>({false, true, true, false}));
  }

  {
    SAT2 sat2(DNF{{X(0), X(1) >= !X(0)}});
    ASSERT_FALSE(sat2.Unsat());
    ASSERT_EQ(sat2.Assignment(), vector<bool>({true, false}));
  }

  {
    SAT2 sat2(DNF{{X(0), !X(0)}});
    ASSERT_TRUE(sat2.Unsat());
  }

  {
    SAT2 sat2(DNF{{X(0) | X(1), X(2) | !X(3), X(4)}});
    ASSERT_FALSE(sat2.Unsat());
    ASSERT_EQ(sat2.Assignment(), vector<bool>({true, true, true, false, true}));
  }

  {
    SAT2 sat2(DNF{{X(0) | !X(1), X(1) | !X(2), X(2) | !X(0)}});
    ASSERT_FALSE(sat2.Unsat());
    ASSERT_EQ(sat2.Assignment(), vector<bool>({true, true, true}));
  }

  {
    SAT2 sat2(DNF{{X(0) | !X(1), X(0) | !X(2), X(1) | X(2), !X(0) | !X(3),
                   X(3) | !X(1), X(3) | !X(2)}});
    ASSERT_TRUE(sat2.Unsat());
  }
}
}  // namespace

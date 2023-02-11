#pragma once

#include "solvers/dlx.h"

#include <array>
#include <cassert>
#include <cstdint>

namespace algo::solvers {
class Sudoku {
public:
  static constexpr uint32_t N = 9;

  using Board = std::array<std::array<uint32_t, N>, N>;

  template <typename Fn>
  static void ForEach(Fn&& fn) {
    for (uint32_t r = 0; r < N; ++r) {
      for (uint32_t c = 0; c < N; ++c) {
        const auto b = 3 * (r / 3) + (c / 3);
        fn(r, c, b);
      }
    }
  }

  template <typename Cont, typename Value>
  static bool AllEqual(const Cont& c, const Value& v) {
    for (const auto& i : c) {
      if (i != v)
        return false;
    }
    return true;
  }

  static bool IsGood(const Board& board) {
    std::array<uint32_t, N> rows{};
    std::array<uint32_t, N> cols{};
    std::array<uint32_t, N> boxs{};

    bool good = true;
    ForEach([&](uint32_t r, uint32_t c, uint32_t b) {
      if (board[r][c] < 1 || board[r][c] > N) {
        good = false;
        return;
      }
      const auto bit = static_cast<uint32_t>(1) << (board[r][c] - 1);
      if (rows[r] & bit) {
        good = false;
        return;
      }
      if (cols[c] & bit) {
        good = false;
        return;
      }
      if (boxs[b] & bit) {
        good = false;
        return;
      }
      rows[r] |= bit;
      cols[c] |= bit;
      boxs[b] |= bit;
    });

    const uint32_t FULL_MASK = 0x1ff;
    return good && AllEqual(rows, FULL_MASK) && AllEqual(cols, FULL_MASK) && AllEqual(boxs, FULL_MASK);
  }

  template <typename Fn>
  static void Solve(const Board& board, Fn&& fn) {
    std::array<uint32_t, N> rows{};
    std::array<uint32_t, N> cols{};
    std::array<uint32_t, N> boxs{};

    ForEach([&](uint32_t r, uint32_t c, uint32_t b) {
      if (board[r][c] < 1 || board[r][c] > N)
        return;

      const auto bit = static_cast<uint32_t>(1) << (board[r][c] - 1);
      assert((rows[r] & bit) == 0);
      assert((cols[c] & bit) == 0);
      assert((boxs[b] & bit) == 0);

      rows[r] |= bit;
      cols[c] |= bit;
      boxs[b] |= bit;
    });

    enum { CELL, ROW, COL, BOX, NUM_DIMS };
    DLX::DimTask<NUM_DIMS> task{N * N, N * N, N * N, N * N};

    ForEach([&](uint32_t r, uint32_t c, uint32_t b) {
      if (board[r][c] >= 1 && board[r][c] <= N) {
        task.template SetCover<CELL>(r * N + c);
        return;
      }

      for (uint32_t v = 0; v < N; ++v) {
        const auto bit = static_cast<uint32_t>(1) << v;
        if (rows[r] & bit)
          task.template SetCover<ROW>(r * N + v);
        if (cols[c] & bit)
          task.template SetCover<COL>(c * N + v);
        if (boxs[b] & bit)
          task.template SetCover<BOX>(b * N + v);

        task.AddOption()
            .template SetItems<CELL>(r * N + c)
            .template SetItems<ROW>(r * N + v)
            .template SetItems<COL>(c * N + v)
            .template SetItems<BOX>(b * N + v);
      }
    });

    DLX::Solve(task, [&](const uint32_t* options, uint32_t n) {
      Board result = board;
      for (uint32_t i = 0; i < n; ++i) {
        const auto& option = task.GetOption(options[i]);

        const auto r = option[ROW][0] / N;
        const auto c = option[COL][0] / N;
        const auto v = option[ROW][0] % N;

        result[r][c] = v + 1;
      }

      fn(result);
    });
  }
};
}  // namespace algo::solvers

#pragma once

#include "solvers/dlx.h"

#include <algorithm>
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
    return std::all_of(std::begin(c), std::end(c), [&v](const auto& i) { return i == v; });
  }

  template <bool IsComplete>
  static bool GetFootprint(const Board& board,
                           std::array<uint32_t, N>& rows,
                           std::array<uint32_t, N>& cols,
                           std::array<uint32_t, N>& boxs) {
    bool good = true;

    ForEach([&](uint32_t r, uint32_t c, uint32_t b) {
      if (board[r][c] < 1 || board[r][c] > N) {
        if (IsComplete)
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

    return good;
  }

  template <bool IsComplete>
  static bool IsGood(const Board& board) {
    std::array<uint32_t, N> rows{};
    std::array<uint32_t, N> cols{};
    std::array<uint32_t, N> boxs{};
    if (!GetFootprint<IsComplete>(board, rows, cols, boxs))
      return false;
    if (!IsComplete)
      return true;

    const uint32_t FULL_MASK = 0x1ff;
    return AllEqual(rows, FULL_MASK) && AllEqual(cols, FULL_MASK) && AllEqual(boxs, FULL_MASK);
  }

  template <typename Fn>
  static void Solve(const Board& board, Fn&& fn) {
    if (!IsGood</* IsComplete= */ false>(board))
      return;

    enum { CELL, ROW, COL, BOX, NUM_DIMS };
    DLX::DimTask<NUM_DIMS> task{N * N, N * N, N * N, N * N};

    ForEach([&](uint32_t r, uint32_t c, uint32_t b) {
      if (board[r][c] >= 1 && board[r][c] <= N) {
        const auto v = board[r][c] - 1;
        task.template SetCover<CELL>(r * N + c);
        task.template SetCover<ROW>(r * N + v);
        task.template SetCover<COL>(c * N + v);
        task.template SetCover<BOX>(b * N + v);
      } else {
        for (uint32_t v = 0; v < N; ++v) {
          task.AddOption()
              .template SetItems<CELL>(r * N + c)
              .template SetItems<ROW>(r * N + v)
              .template SetItems<COL>(c * N + v)
              .template SetItems<BOX>(b * N + v);
        }
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

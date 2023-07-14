#pragma once

#include <concepts>
#include <memory>
#include <tuple>

#include "board.hpp"
#include "board_stack.hpp"
#include "move.hpp"
#include "move_generator.hpp"
#include "transposition.hpp"
#include "utils.hpp"

namespace BobChess
{

template <typename T>
concept evaluator = requires(const Board& bd) {
  std::is_integral<decltype(T::eval(bd))>();
};

template <evaluator the_eval>
class algorithm
{
 public:
  algorithm() = delete;

  algorithm(const Board& bd, TTable& tt);

  std::tuple<const Move&, u64, int> get_best_move(int depth);

 private:
  TTable& m_tt;
  BoardStack m_bs;

  std::unique_ptr<u64> m_node_count;

  int quescence(int depth, int alpha, int beta, int color);
  int negamax(int depth, int alpha, int beta, int color);

  /// @brief arbitrarily large number to use in place of infinity
  static constexpr int infinity = 1000000;

  /// @brief checkmate value
  static constexpr int game_over = 100000;
};

}  // namespace BobChess

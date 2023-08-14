#pragma once

#include <atomic>
#include <concepts>
#include <functional>
#include <memory>
#include <tuple>

#include "board.hpp"
#include "board_stack.hpp"
#include "move.hpp"
#include "move_generator.hpp"
#include "transposition.hpp"
#include "utils.hpp"

namespace BeepBoop
{

class Algorithm
{
 public:
  Algorithm() = delete;

  ~Algorithm() = default;

  Algorithm(Board&& bd, std::function<int(Board)> eval, TTable& table);

  /// @brief evaluate a move
  /// @param depth
  /// @return <value, nodecount>
  std::tuple<int, std::size_t> evaluate_move(int depth);

  int quescence(int depth, int alpha, int beta);
  int negamax(int depth, int alpha, int beta);

  std::size_t get_size() const noexcept;

 private:
  TTable& m_tt;
  BoardStack m_bs;
  std::function<int(Board)>& m_eval;

  std::size_t m_count{};

  /// @brief arbitrarily large number to use in place of infinity
  static constexpr int infinity = 500000;

  /// @brief checkmate value
  static constexpr int game_over = 100000;

  static constexpr int full_depth = 3;

  static constexpr int LMR_R = 3;

  static constexpr int q_depth = 3;

  int current_depth{0};

  int current_guess{0};
};

}  // namespace BeepBoop

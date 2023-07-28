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

namespace BobChess
{

class Algorithm
{
 public:
  Algorithm() = delete;

  ~Algorithm() = default;

  Algorithm(Board&& bd, std::function<int(Board)> eval, TTable&& table = TTable{});

  /// @brief evaluate a move
  /// @param depth
  /// @return <value, nodecount>
  std::tuple<int, std::size_t> evaluate_move(int depth);

  const TTable* transposition_ptr() const noexcept;

  void set_transposition(TTable* table) noexcept;

 private:
  TTable m_tt;
  BoardStack m_bs;
  std::function<int(Board)>& m_eval;

  std::size_t m_count{};

  int quescence(int depth, int alpha, int beta);
  int negamax(int depth, int alpha, int beta);

  /// @brief arbitrarily large number to use in place of infinity
  static constexpr int infinity = 500000;

  /// @brief checkmate value
  static constexpr int game_over = 100000;

  static constexpr int full_depth = 3;

  static constexpr int LMR_R = 3;

  static constexpr int q_depth = 2;

  int current_depth{0};

  int current_guess{0};
};

}  // namespace BobChess

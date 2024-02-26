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
  /// @brief constructor (do not allow default construction)
  Algorithm() = delete;

  /// @brief constructor (default move construction)
  Algorithm(Algorithm&& other) = default;

  /// @brief constructor (do not allow copy construction)
  Algorithm(const Algorithm& other) = delete;

  /// @brief destructor
  ~Algorithm() = default;

  /// @brief constructor
  /// @param bd the board
  /// @param eval a function to determine evaluation
  /// @param table a reference to a transposition table
  Algorithm(Board&& bd, std::function<int(Board)> eval, TTable& table);

  /// @brief evaluate a move
  /// @param depth
  /// @return <value, nodecount>
  std::tuple<MoveList, std::size_t> evaluate_position(int depth);

  /// @brief quiescence search
  /// @param depth the max depth
  /// @param alpha the lower bound
  /// @param beta the upper bound
  /// @return the quiesced value
  int quescence(int depth, int alpha, int beta);

  /// @brief negamax search
  /// @param depth the max depth
  /// @param alpha the lower bound
  /// @param beta the upper bound
  /// @return the negamax value
  int negamax(int depth, int alpha, int beta);

  /// @brief get the node count
  /// @return a size_t number
  std::size_t get_size() const noexcept;

 private:
  TTable&                    m_tt;
  BoardStack                 m_bs;
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

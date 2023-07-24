#pragma once

#include <tuple>

#include "algorithm_chess.hpp"

namespace BobChess
{

class MoveFinder
{
 public:
  /// @brief get the best move
  /// @param bd
  /// @param depth
  /// @param eval
  /// @return
  static std::tuple<Move, std::size_t> get_best_move(const Board& bd, int depth, std::function<int(Board)> eval);

  /// @brief get the best move
  /// @param bd
  /// @param milliseconds
  /// @param eval
  /// @return
  static std::tuple<Move, std::size_t> get_best_move_time(const Board& bd, double milliseconds,
                                                          std::function<int(Board)> eval);
};

}  // namespace BobChess
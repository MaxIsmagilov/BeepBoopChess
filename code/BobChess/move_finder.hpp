#pragma once

#include <tuple>

#include "algorithm_chess.hpp"

namespace BobChess
{

class MoveFinder
{
 public:
  static std::tuple<Move, std::size_t> get_best_move(const Board& bd, int depth, std::function<int(Board)> eval);

  static std::tuple<Move, std::size_t> get_best_move_time(const Board& bd, double milliseconds,
                                                          std::function<int(Board)> eval);
};

}  // namespace BobChess
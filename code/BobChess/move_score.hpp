#pragma once

#include "board.hpp"
#include "move.hpp"

namespace BobChess
{

class MoveScorer
{
 public:
  static int score(const Board& bd, const Move& move);

 private:
  static constexpr int capture_weight = 2;
  static constexpr int check_bias = 2000;
  static constexpr int castle_bias = 200;
  static constexpr int promotion_bias[6] = {0, 400, 500, 600, 700, 0};
  static constexpr int MVVLVA[6][6] = {{40, 39, 38, 37, 36, 35},  //
                                       {51, 50, 49, 48, 47, 46},  //
                                       {62, 61, 60, 59, 58, 57},  //
                                       {73, 72, 71, 70, 69, 68},  //
                                       {84, 83, 82, 81, 80, 79},  //
                                       {95, 94, 93, 92, 91, 90}};
};

}  // namespace BobChess

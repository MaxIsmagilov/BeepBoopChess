#include "move_score.hpp"

#include "move_generator.hpp"

namespace BobChess
{

int MoveScorer::score(const Board& bd, const Move& move) {
  int score{0};
  if (move.is_capture()) {
    if (move.is_enpassant()) [[unlikely]]
      score += 40 * capture_weight;
    else {
      int movedpiece = move.get_piece();
      int endsquare = move.get_end();
      int capturedpiece;
      int scan = (bd.side_to_move()) ? 0 : 6;
      for (int i = 0; i < 6; ++i) {
        if (bd[i + scan] & utils::set_bit_true(0ULL, endsquare)) {
          capturedpiece = i;
          break;
        }
      }
      score += MVVLVA[movedpiece][capturedpiece] * capture_weight;
    }
  }
  score += promotion_bias[move.get_promote() * move.is_promote()];
  score += castle_bias * move.is_castle();
  if (MoveGenerator::in_check(bd.move_copy(move), !bd.side_to_move())) score += check_bias;
  return score;
}

}  // namespace BobChess

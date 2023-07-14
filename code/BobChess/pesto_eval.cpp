
#include "pesto_eval.hpp"

#include <algorithm>
#include <sstream>

#include "board.hpp"

namespace BobChess
{

namespace Macroes
{
#define OTHER_SIDE(side)   (side ^ 1)
#define FLIP(sq)           ((sq) ^ 56)
#define WHITE_PIECE(piece) (2 * piece + utils::WHITE)
#define BLACK_PIECE(piece) (2 * piece + utils::BLACK)
}  // namespace Macroes

void Evaluator::initialize_evaluation() noexcept {
  int piece, p, square;
  for (p = utils::PAWN, piece = WHITE_PIECE(utils::PAWN); p <= utils::KING; piece += 2, p++) {
    for (square = 0; square < 64; square++) {
      mg_table[piece][square] = mg_value[p] + mg_pesto_table[p][square];
      eg_table[piece][square] = eg_value[p] + eg_pesto_table[p][square];
      mg_table[piece + 1][square] = mg_value[p] + mg_pesto_table[p][FLIP(square)];
      eg_table[piece + 1][square] = eg_value[p] + eg_pesto_table[p][FLIP(square)];
    }
  }
}

int Evaluator::eval(const Board& bd) noexcept {
  int game_phase = 0;

  uint64_t board_copy[12] = {bd[0], bd[1], bd[2], bd[3], bd[4], bd[5], bd[6], bd[7], bd[8], bd[9], bd[10], bd[11]};

  int mg[2] = {0, 0};
  int eg[2] = {0, 0};

  // evaluate white pieces
  std::for_each(board_copy, board_copy + 6, [&](uint64_t& bb) mutable {
    while (bb) {
      const int p = &bb - board_copy;
      const int j = utils::LSB_index(bb);
      bb = utils::set_bit_false(bb, j);
      mg[utils::WHITE] += mg_table[WHITE_PIECE(p)][j];
      eg[utils::WHITE] += eg_table[WHITE_PIECE(p)][j];
      game_phase += game_phase_inc[WHITE_PIECE(p)];
    }
  });

  // evaluate black pieces
  std::for_each(board_copy + 6, board_copy + 12, [&](uint64_t& bb) mutable {
    while (bb) {
      const int p = &bb - (board_copy + 6);
      const int j = utils::LSB_index(bb);
      bb = utils::set_bit_false(bb, j);
      mg[utils::BLACK] += mg_table[BLACK_PIECE(p)][j];
      eg[utils::BLACK] += eg_table[BLACK_PIECE(p)][j];
      game_phase += game_phase_inc[BLACK_PIECE(p)];
    }
  });

  // tapered evaluation
  const int mg_score = mg[utils::WHITE] - mg[utils::BLACK];
  const int eg_score = eg[utils::WHITE] - eg[utils::BLACK];
  const int mg_phase = (game_phase > 24) ? 24 : game_phase;
  const int eg_phase = 24 - mg_phase;
  return ((mg_score * mg_phase) + (eg_score * eg_phase)) / 24;
}

}  // namespace BobChess
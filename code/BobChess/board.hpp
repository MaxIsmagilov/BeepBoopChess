#pragma once

// C++ standard library:

// BobChess headers:
#include "move.hpp"
#include "utils.hpp"

namespace BobChess {

class Board {
 public:
  void make_move(const Move& mv);

 private:
  u64 m_board[12];

  unsigned char m_enpassant : 4;

  bool m_castle_WK : 1;
  bool m_castle_WQ : 1;

  bool m_castle_BK : 1;
  bool m_castle_BQ : 1;
  bool m_side      : 1;
};

}  // namespace BobChess

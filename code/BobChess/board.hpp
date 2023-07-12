#pragma once

// C++ standard library:

// BobChess headers:
#include "move.hpp"
#include "utils.hpp"

namespace BobChess {

class Board {
 public:
  enum Castle { WHITE_SHORT, WHITE_LONG, QUEEN_SHORT, QUEEN_LONG };
  void make_move(const Move& mv) noexcept;
  u64 operator[](int index) const noexcept;
  u64 white_occ() const noexcept;
  u64 black_occ() const noexcept;
  u64 all_occ() const noexcept;

  bool castle_available(int castle);

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

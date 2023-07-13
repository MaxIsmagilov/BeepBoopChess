#pragma once

// C++ standard library:
#include <string>

// BobChess headers:
#include "move.hpp"
#include "utils.hpp"

namespace BobChess
{

class Board
{
 public:
  enum Castle { WHITE_SHORT, WHITE_LONG, QUEEN_SHORT, QUEEN_LONG };

  void make_move(const Move& mv) noexcept;
  void import_FEN(const char* FEN);
  void flip_side() noexcept;

  u64 operator[](int index) const noexcept;
  u64 white_occ() const noexcept;
  u64 black_occ() const noexcept;
  u64 all_occ() const noexcept;
  bool side_to_move() const noexcept;

  bool castle_available(int castle);

  std::string debug_print() const;
  std::string nice_print() const;

 private:
  u64 m_board[12];

  unsigned int m_halfmoves : 16;
  unsigned int m_fullmoves : 16;

  unsigned char m_enpassant : 8;

  bool m_castle_WK : 1;
  bool m_castle_WQ : 1;

  bool m_castle_BK : 1;
  bool m_castle_BQ : 1;
  bool m_side      : 1;
};

}  // namespace BobChess

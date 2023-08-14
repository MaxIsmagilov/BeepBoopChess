#pragma once

// C++ standard library:
#include <string>

// BeepBoop headers:
#include "move.hpp"
#include "utils.hpp"

namespace BeepBoop
{

class Board
{
 public:
  enum Castle { WHITE_SHORT, WHITE_LONG, BLACK_SHORT, BLACK_LONG };

  /// @brief make a move
  /// @param mv
  void make_move(const Move& mv) noexcept;

  /// @brief import a FEN string
  /// @param FEN
  void import_FEN(const char* FEN);

  /// @brief flip the moving side
  void flip_side() noexcept;

  /// @brief move-copy.
  /// @param mv
  /// @return a copy of the board in which the board has moved by mv
  Board move_copy(const Move& mv) const noexcept;

  /// @brief get a bitboard
  /// @param index
  /// @return a bitboard
  u64 operator[](int index) const noexcept;

  /// @brief return white's occupancy
  /// @return a bitboard
  u64 white_occ() const noexcept;

  /// @brief return black's occupancy
  /// @return a bitboard
  u64 black_occ() const noexcept;

  /// @brief return white's and black's occupancy
  /// @return a bitboard
  u64 all_occ() const noexcept;

  /// @brief get the side to move
  /// @return bool, true if white
  bool side_to_move() const noexcept;

  /// @brief checks whether a castle is available
  /// @param castle a Board::castle
  /// @return the availability
  bool castle_available(Castle castle) const;

  /// @brief gets the enpassant square
  /// @return the location of the square if available, else 255
  u32 enpassant_square() const;

  /// @brief get the halfmoves
  /// @return a u32
  u32 halfmoves() const noexcept;

  /// @brief get the fullmoves
  /// @return a u32
  u32 fullmoves() const noexcept;

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

}  // namespace BeepBoop

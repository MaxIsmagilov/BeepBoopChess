#pragma once

// C++ standard library:

// BobChess headers:
#include "utils.hpp"

namespace BobChess
{

class Move
{
 public:
  Move() noexcept;

  Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool castle, bool is_promote, bool enpassant,
       bool is_double);

  Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool castle, bool is_promote, bool enpassant,
       bool is_double, unsigned int heuristic);

  void set_heuristic(unsigned int heuristic) noexcept;

  u8 get_start() const noexcept;
  u8 get_end() const noexcept;
  u8 get_piece() const noexcept;
  u8 get_promote() const noexcept;
  bool is_capture() const noexcept;
  bool is_castle() const noexcept;
  bool is_promote() const noexcept;
  bool is_enpassant() const noexcept;
  bool is_double_push() const noexcept;

  bool operator>(const Move& other) const noexcept;
  bool operator<(const Move& other) const noexcept;

 private:
  unsigned int m_heuristic : 16;

  u8 m_start       : 7;
  u8 m_end         : 7;
  u8 m_piece       : 4;
  u8 m_promote     : 4;
  u8 m_capture     : 1;
  u8 m_castle      : 1;
  u8 m_is_promote  : 1;
  u8 m_enpassant   : 1;
  u8 m_double_push : 1;
};

}  // namespace BobChess

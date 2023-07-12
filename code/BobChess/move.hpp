#pragma once

// C++ standard library:

// BobChess headers:
#include "utils.hpp"

namespace BobChess {

class Move {
 public:
  Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool is_promote, bool enpassant, bool is_double);

  Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool is_promote, bool enpassant, bool is_double,
       unsigned int heuristic);

  void set_heuristic(unsigned int heuristic);

  u8 get_start() const;
  u8 get_end() const;
  u8 get_piece() const;
  u8 get_promote() const;
  bool is_capture() const;
  bool is_promote() const;
  bool is_enpassant() const;
  bool is_double_push() const;

 private:
  unsigned int m_heuristic : 16;

  u8 m_start       : 7;
  u8 m_end         : 7;
  u8 m_piece       : 4;
  u8 m_promote     : 4;
  u8 m_capture     : 1;
  u8 m_is_promote  : 1;
  u8 m_enpassant   : 1;
  u8 m_double_push : 1;
};

}  // namespace BobChess

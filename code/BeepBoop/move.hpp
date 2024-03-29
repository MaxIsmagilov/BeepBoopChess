#pragma once

// C++ standard library:
#include <string>

// BeepBoop headers:
#include "utils.hpp"

namespace BeepBoop
{

class Move
{
 public:
  Move() noexcept;

  Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool castle, bool is_promote, bool enpassant,
       bool is_double);

  Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool castle, bool is_promote, bool enpassant,
       bool is_double, int heuristic);

  Move(Move&& m)                 = default;
  Move(const Move& m)            = default;
  Move& operator=(const Move& m) = default;

  void set_heuristic(int heuristic) noexcept;

  u8   get_start() const noexcept;
  u8   get_end() const noexcept;
  u8   get_piece() const noexcept;
  u8   get_promote() const noexcept;
  bool is_capture() const noexcept;
  bool is_castle() const noexcept;
  bool is_promote() const noexcept;
  bool is_enpassant() const noexcept;
  bool is_double_push() const noexcept;

  int get_heuristic() const noexcept;

  bool is_reduceable() const noexcept;

  bool operator>(const Move& other) const noexcept;
  bool operator<(const Move& other) const noexcept;

  std::string to_string() const;
  std::string print_value() const;

  void flag();
  bool flagged() const;

 private:
  int m_heuristic;

  u8 m_start       : 7;
  u8 m_end         : 7;
  u8 m_piece       : 4;
  u8 m_promote     : 4;
  u8 m_capture     : 1;
  u8 m_castle      : 1;
  u8 m_is_promote  : 1;
  u8 m_enpassant   : 1;
  u8 m_double_push : 1;
  u8 m_flag        : 1;
};

}  // namespace BeepBoop

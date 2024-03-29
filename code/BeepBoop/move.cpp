#include "move.hpp"

// C++ standard library:
#include <cmath>

// BeepBoop headers:
#include "utils.hpp"

namespace BeepBoop
{
Move::Move() noexcept
    : m_start{0},
      m_end{0},
      m_piece{0},
      m_promote{0},
      m_capture{0},
      m_castle{0},
      m_is_promote{0},
      m_enpassant{0},
      m_double_push{0},
      m_flag{0} {}

Move::Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool castle, bool is_promote, bool enpassant,
           bool is_double)
    : m_start{start},
      m_end{end},
      m_piece{piece},
      m_promote{promote},
      m_capture{capture},
      m_castle{castle},
      m_is_promote{is_promote},
      m_enpassant{enpassant},
      m_double_push{is_double},
      m_flag{0} {}

Move::Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool castle, bool is_promote, bool enpassant,
           bool is_double, int heuristic)
    : m_heuristic{heuristic},
      m_start{start},
      m_end{end},
      m_piece{piece},
      m_promote{promote},
      m_capture{capture},
      m_castle{castle},
      m_is_promote{is_promote},
      m_enpassant{enpassant},
      m_double_push{is_double},
      m_flag{0} {}

void Move::set_heuristic(int heuristic) noexcept { m_heuristic = heuristic; }

u8 Move::get_start() const noexcept { return static_cast<u8>(m_start); }

u8 Move::get_end() const noexcept { return static_cast<u8>(m_end); }

u8 Move::get_piece() const noexcept { return static_cast<u8>(m_piece); }

u8 Move::get_promote() const noexcept { return static_cast<u8>(m_promote); }

bool Move::is_capture() const noexcept { return static_cast<bool>(m_capture); }

bool Move::is_castle() const noexcept { return static_cast<bool>(m_castle); }

bool Move::is_promote() const noexcept { return static_cast<bool>(m_is_promote); }

bool Move::is_enpassant() const noexcept { return static_cast<bool>(m_enpassant); }

bool Move::is_double_push() const noexcept { return static_cast<bool>(m_double_push); }

int Move::get_heuristic() const noexcept { return m_heuristic; }

bool Move::is_reduceable() const noexcept { return !(m_capture || m_is_promote); }

bool Move::operator>(const Move& other) const noexcept { return m_heuristic > other.m_heuristic; }

bool Move::operator<(const Move& other) const noexcept { return m_heuristic < other.m_heuristic; }

std::string Move::to_string() const {
  if (!m_start && !m_end) return "0000";
  char        pieces[] = {'p', 'n', 'b', 'r', 'q'};
  std::string s        = "";
  s.push_back('a' + (m_start % 8));
  s.push_back('8' - (m_start / 8));
  s.push_back('a' + (m_end % 8));
  s.push_back('8' - (m_end / 8));
  if (m_promote) s.push_back(pieces[m_promote]);
  return s;
}

std::string Move::print_value() const {
  if (std::abs(m_heuristic) >= 10000) {
    const auto absval  = std::abs(m_heuristic);
    const auto mate_in = (100000 - absval) * (absval / m_heuristic);
    return ("Mate in" + std::to_string(mate_in));
  } else {
    return std::to_string(m_heuristic / 100.0);
  }
}

void Move::flag() { m_flag = true; }
bool Move::flagged() const { return m_flag; }

}  // namespace BeepBoop
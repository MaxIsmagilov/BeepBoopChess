#include "move.hpp"

// C++ standard library:

// BobChess headers:
#include "utils.hpp"

namespace BobChess {

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
      m_double_push{is_double} {}

Move::Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool castle, bool is_promote, bool enpassant,
           bool is_double, unsigned int heuristic)
    : m_heuristic{heuristic},
      m_start{start},
      m_end{end},
      m_piece{piece},
      m_promote{promote},
      m_capture{capture},
      m_castle{castle},
      m_is_promote{is_promote},
      m_enpassant{enpassant},
      m_double_push{is_double} {}

void Move::set_heuristic(unsigned int heuristic) noexcept { m_heuristic = heuristic; }

u8 Move::get_start() const noexcept { return static_cast<u8>(m_start); }

u8 Move::get_end() const noexcept { return static_cast<u8>(m_end); }

u8 Move::get_piece() const noexcept { return static_cast<u8>(m_piece); }

u8 Move::get_promote() const noexcept { return static_cast<u8>(m_promote); }

bool Move::is_capture() const noexcept { return static_cast<bool>(m_capture); }

bool Move::is_castle() const noexcept { return static_cast<bool>(m_castle); }

bool Move::is_promote() const noexcept { return static_cast<bool>(m_is_promote); }

bool Move::is_enpassant() const noexcept { return static_cast<bool>(m_enpassant); }

bool Move::is_double_push() const noexcept { return static_cast<bool>(m_double_push); }

}  // namespace BobChess
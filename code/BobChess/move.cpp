#include "move.hpp"

// C++ standard library:

// BobChess headers:
#include "utils.hpp"

namespace BobChess {

Move::Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool is_promote, bool enpassant, bool is_double)
    : m_start{start},
      m_end{end},
      m_piece{piece},
      m_promote{promote},
      m_capture{capture},
      m_is_promote{is_promote},
      m_enpassant{enpassant},
      m_double_push{is_double} {}

Move::Move(u8 start, u8 end, u8 piece, u8 promote, bool capture, bool is_promote, bool enpassant, bool is_double,
           unsigned int heuristic)
    : m_heuristic{heuristic},
      m_start{start},
      m_end{end},
      m_piece{piece},
      m_promote{promote},
      m_capture{capture},
      m_is_promote{is_promote},
      m_enpassant{enpassant},
      m_double_push{is_double} {}

void Move::set_heuristic(unsigned int heuristic) { m_heuristic = heuristic; }

u8 Move::get_start() const { return static_cast<u8>(m_start); }

u8 Move::get_end() const { return static_cast<u8>(m_end); }

u8 Move::get_piece() const { return static_cast<u8>(m_piece); }

u8 Move::get_promote() const { return static_cast<u8>(m_promote); }

bool Move::is_capture() const { return static_cast<bool>(m_capture); }

bool Move::is_promote() const { return static_cast<bool>(m_is_promote); }

bool Move::is_enpassant() const { return static_cast<bool>(m_enpassant); }

bool Move::is_double_push() const { return static_cast<bool>(m_double_push); }

}  // namespace BobChess
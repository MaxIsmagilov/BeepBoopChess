#include "board.hpp"

//
namespace BobChess {

void Board::make_move([[maybe_unused]] const Move& mv) noexcept {}

u64 Board::operator[](int index) const noexcept { return m_board[index]; }

u64 Board::white_occ() const noexcept {
  return m_board[0] | m_board[1] | m_board[2] | m_board[3] | m_board[4] | m_board[5];
}
u64 Board::black_occ() const noexcept {
  return m_board[6] | m_board[7] | m_board[8] | m_board[9] | m_board[10] | m_board[11];
}
u64 Board::all_occ() const noexcept {
  return m_board[0] | m_board[1] | m_board[2] | m_board[3] | m_board[4] | m_board[5] | m_board[6] | m_board[7] |
         m_board[8] | m_board[9] | m_board[10] | m_board[11];
}

bool Board::castle_available(int castle) {
  switch (castle) {
    case 0:
      return m_castle_WK;
    case 1:
      return m_castle_WQ;
    case 2:
      return m_castle_BK;
    case 3:
      return m_castle_BQ;
  }
  return false;
}

}  // namespace BobChess
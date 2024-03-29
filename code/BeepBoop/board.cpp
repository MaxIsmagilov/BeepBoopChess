#include <iostream>

#include "board.hpp"

namespace BeepBoop
{

void Board::make_move(const Move& mv) noexcept {
  if (!(mv.get_end() || mv.get_start())) [[unlikely]] {
    m_side ^= 1u;
    return;
  }

  m_halfmoves++;
  auto piecemod = (m_side) ? 0 : 6;

  if (mv.is_capture() || !mv.get_piece()) m_halfmoves = 0;

  if (m_side ^= 1u) m_fullmoves++;

  if (mv.get_piece() == utils::PAWN && mv.is_double_push())
    m_enpassant = (mv.get_start() + mv.get_end()) / 2;
  else
    m_enpassant = 255;

  m_board[mv.get_piece() + piecemod] &= ~(1ULL << mv.get_start());

  if (mv.is_capture()) {
    for (int i = 0; i < 12; ++i) {
      m_board[i] &= ~(1ULL << mv.get_end());
    }
  }

  if (mv.is_promote())
    m_board[mv.get_promote() + piecemod] |= (1ULL << mv.get_end());
  else
    m_board[mv.get_piece() + piecemod] |= (1ULL << mv.get_end());

  if (mv.is_castle()) {
    if (m_side == 0) {
      m_board[3] |= (1ULL << ((mv.get_end() + mv.get_start()) / 2));
      m_board[3] &= ~(1ULL << ((mv.get_end() > mv.get_start()) ? utils::_H1 : utils::_A1));
    }

    else {
      m_board[9] |= (1ULL << ((mv.get_end() + mv.get_start()) / 2));
      m_board[9] &= ~(1ULL << ((mv.get_end() > mv.get_start()) ? utils::_H8 : utils::_A8));
    }
  }

  if (mv.is_enpassant()) {
    if (m_side) {
      m_board[0] &= ~(1ULL << (mv.get_end() - 8));
    } else {
      m_board[6] &= ~(1ULL << (mv.get_end() + 8));
    }
  }

  if (mv.get_start() == utils::_A1 || mv.get_end() == utils::_A1) {
    m_castle_WQ = false;
  }

  if (mv.get_start() == utils::_A8 || mv.get_end() == utils::_A8) {
    m_castle_BQ = false;
  }

  if (mv.get_start() == utils::_H1 || mv.get_end() == utils::_H1) {
    m_castle_WK = false;
  }

  if (mv.get_start() == utils::_H8 || mv.get_end() == utils::_H8) {
    m_castle_BK = false;
  }

  if (mv.get_piece() == utils::KING) {
    if (!m_side) {
      m_castle_WK = false;
      m_castle_WQ = false;
    } else {
      m_castle_BK = false;
      m_castle_BQ = false;
    }
  }
}

void Board::import_FEN(const char* FEN) {
  char arr[64];
  int  board_index = 0;
  int  str_index;

  for (str_index = 0; FEN[str_index] != ' '; ++str_index) {
    if (FEN[str_index] == '/')
      continue;
    else if (std::isdigit(FEN[str_index])) {
      auto num = std::stoi(FEN + str_index);
      while (num) {
        arr[board_index] = '\0';
        ++board_index;
        --num;
      }
    } else {
      arr[board_index] = FEN[str_index];
      ++board_index;
    }
  }
  for (int i = 0; i < 12; i++) {
    m_board[i] = 0ULL;
  }
  for (int pos = 0; pos < 64; pos++) {
    u64 itr = 1ULL << pos;
    switch (arr[pos]) {
      case 'P':
        m_board[0] |= itr;
        break;
      case 'N':
        m_board[1] |= itr;
        break;
      case 'B':
        m_board[2] |= itr;
        break;
      case 'R':
        m_board[3] |= itr;
        break;
      case 'Q':
        m_board[4] |= itr;
        break;
      case 'K':
        m_board[5] |= itr;
        break;

      case 'p':
        m_board[6] |= itr;
        break;
      case 'n':
        m_board[7] |= itr;
        break;
      case 'b':
        m_board[8] |= itr;
        break;
      case 'r':
        m_board[9] |= itr;
        break;
      case 'q':
        m_board[10] |= itr;
        break;
      case 'k':
        m_board[11] |= itr;
        break;
    }
  }
  if (FEN[++str_index] == 'w')
    m_side = 1;
  else
    m_side = 0;
  str_index += 2;
  m_castle_WK = false;
  m_castle_WQ = false;
  m_castle_BK = false;
  m_castle_BQ = false;
  for (str_index = str_index; FEN[str_index] != ' '; ++str_index) {
    switch (FEN[str_index]) {
      case 'K':
        m_castle_WK = true;
        break;
      case 'Q':
        m_castle_WQ = true;
        break;
      case 'k':
        m_castle_BK = true;
        break;
      case 'q':
        m_castle_BQ = true;
        break;
    }
  }
  if (FEN[++str_index] != '-') {
    m_enpassant = 0;
    m_enpassant += FEN[str_index] - 'a';
    m_enpassant += 8 * (7 - (FEN[++str_index] - '1'));
  } else
    m_enpassant = 255;
  ++str_index;
  // std::cerr << FEN + str_index << '\n';
  m_halfmoves = std::stoi(FEN + (++str_index));
  int len     = (m_halfmoves / 10) + 1;
  // std::cerr << len << '\n';
  m_fullmoves = std::stoi(FEN + (++str_index) + len);
}

void Board::flip_side() noexcept { m_side ^= 1; }

Board Board::move_copy(const Move& mv) const noexcept {
  Board bdnew = *this;
  bdnew.make_move(mv);
  return bdnew;
}

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

bool Board::side_to_move() const noexcept { return m_side; }

bool Board::castle_available(Castle castle) const {
  switch (castle) {
    case WHITE_SHORT:
      return m_castle_WK;
    case WHITE_LONG:
      return m_castle_WQ;
    case BLACK_SHORT:
      return m_castle_BK;
    case BLACK_LONG:
      return m_castle_BQ;
  }
  return false;
}

u32 Board::enpassant_square() const { return static_cast<u32>(m_enpassant); }
u32 Board::halfmoves() const noexcept { return static_cast<u32>(m_halfmoves); }
u32 Board::fullmoves() const noexcept { return static_cast<u32>(m_fullmoves); }

std::string Board::debug_print() const {
  char        pieces[14] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' ', '#'};
  std::string str{""};
  for (int i = 0; i < 64; ++i) {
    bool found = false;
    for (int j = 0; j < 13; ++j) {
      if (j == 12 && !found) {
        str += pieces[12 + (((i / 8) + i) % 2)];
        str += ' ';
      } else if (j == 12)
        continue;
      else if (utils::get_bit(m_board[j], i)) {
        if (!found) {
          str += pieces[j];
          str += ' ';
          found = true;
        } else {
          str += 'X';
          str += ' ';
        }
      }
    }
    if (i % 8 == 7) str += '\n';
  }
  str += "fullmoves: ";
  str += std::to_string(m_fullmoves);
  str += "\nhalfmoves: ";
  str += std::to_string(m_halfmoves);
  str += (!m_side) ? "\nblack to move\n" : "\nwhite to move\n";
  str += (m_castle_WK) ? 'y' : 'n';
  str += (m_castle_WQ) ? 'y' : 'n';
  str += (m_castle_BK) ? 'y' : 'n';
  str += (m_castle_BQ) ? 'y' : 'n';
  str += "\nenpassant square: ";
  str += ((m_enpassant != 255) ? std::to_string(m_enpassant) : "255 (none)");
  str += '\n';
  return str;
}
std::string Board::nice_print() const { return debug_print(); }

}  // namespace BeepBoop
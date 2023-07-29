#include "board_stack.hpp"

#include <algorithm>

namespace BobChess
{

BoardStack::BoardStack(const Board& bd) : m_boards{bd}, m_moves{Move{}}, size{1} {}

const Board& BoardStack::top() const { return m_boards[size - 1]; }

const Move& BoardStack::lastmove() const { return m_moves[size - 1]; }

void BoardStack::pop() { size -= 1; }

void BoardStack::move(const Move& move) {
  std::copy(m_boards.begin() + size - 1, m_boards.begin() + size, m_boards.begin() + size);
  m_boards[size].make_move(move);
  m_moves[size] = move;
  ++size;
}

void BoardStack::nullmove() {
  std::copy(m_boards.begin() + size - 1, m_boards.begin() + size, m_boards.begin() + size);
  m_boards[size].flip_side();
  m_moves[size] = Move{};
  ++size;
}

}  // namespace BobChess

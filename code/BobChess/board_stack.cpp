#include "board_stack.hpp"

#include <algorithm>

namespace BobChess
{

BoardStack::BoardStack(const Board& bd) : m_array{bd}, size{1} {}

const Board& BoardStack::top() const { return m_array[size - 1]; }

void BoardStack::pop() { size -= 1; }

void BoardStack::move(const Move& move) {
  std::copy(m_array.begin() + size - 1, m_array.begin() + size, m_array.begin() + size);
  m_array[size++].make_move(move);
}

void BoardStack::nullmove() {
  std::copy(m_array.begin() + size - 1, m_array.begin() + size, m_array.begin() + size);
  m_array[size++].flip_side();
}

}  // namespace BobChess

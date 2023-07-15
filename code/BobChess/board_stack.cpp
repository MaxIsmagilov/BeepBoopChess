#include "board_stack.hpp"

#include <algorithm>

namespace BobChess
{

BoardStack::BoardStack(const Board& bd) : size{1}, m_array{bd} {}

const Board& BoardStack::top() const { return m_array[size - 1]; }

void BoardStack::pop() { size -= 1; }

void BoardStack::move(const Move& move) {
  std::copy(m_array.begin() + size - 1, m_array.begin() + size, m_array.begin() + size);
}

}  // namespace BobChess

#include "move_list.hpp"

#include <algorithm>

namespace BobChess
{

const std::array<Move, 256>& MoveList::get_all() const { return m_moves; }

void MoveList::sort() { std::sort(m_moves.begin(), m_moves.begin() + m_last_populated, std::greater<>()); }

void MoveList::partial_sort(int middle)
{
  std::partial_sort(m_moves.begin(), m_moves.begin() + middle, m_moves.begin() + m_last_populated, std::greater<>());
}

void MoveList::move_killer(int num_best, int num_killer) noexcept
{
  std::rotate(m_moves.begin() + num_best, m_moves.begin() + m_last_populated - num_killer,
              m_moves.begin() + m_last_populated);
}

const Move& MoveList::operator[](int index) const { return m_moves[index]; }

void MoveList::push_back(const Move& mv) noexcept { m_moves[++m_last_populated] = mv; }

void MoveList::push_back(Move&& mv) noexcept { m_moves[++m_last_populated] = mv; }

void MoveList::remove(int index) noexcept
{
  std::rotate(m_moves.begin() + index, m_moves.begin() + index + 1, m_moves.begin() + (m_last_populated--));
}

int MoveList::get_size() noexcept { return m_last_populated + 1; }
}  // namespace BobChess
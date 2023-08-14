#include "move_list.hpp"

#include <algorithm>
#include <iostream>

namespace BeepBoop
{

MoveList::MoveList(){};

const std::array<Move, 256>& MoveList::get_all() const noexcept { return m_moves; }

void MoveList::sort() noexcept { std::sort(m_moves.begin(), m_moves.begin() + m_last_populated, std::greater<>()); }

void MoveList::partial_sort(int middle) noexcept {
  std::partial_sort(m_moves.begin(), m_moves.begin() + middle, m_moves.begin() + m_last_populated, std::greater<>());
}

void MoveList::move_killer(int num_best, int num_killer) noexcept {
  std::rotate(m_moves.begin() + num_best, m_moves.begin() + m_last_populated - num_killer,
              m_moves.begin() + m_last_populated);
}

const Move& MoveList::operator[](int index) const noexcept { return m_moves[index]; }

void MoveList::set_heuristic(int index, int heuristic) noexcept { m_moves[index].set_heuristic(heuristic); }

void MoveList::push_back(const Move& mv) noexcept { m_moves[m_last_populated++] = mv; }

void MoveList::push_back(Move&& mv) noexcept { m_moves[m_last_populated++] = mv; }

void MoveList::remove(int index) noexcept {
  std::rotate(m_moves.begin() + index, m_moves.begin() + index + 1, m_moves.begin() + (m_last_populated--));
}

void MoveList::remove(std::vector<int> indeces) noexcept {
  for (u32 i = 0; i < indeces.size(); ++i) {
    this->remove(indeces[i] - i);
  }
}

void MoveList::remove_non_captures() noexcept {
  for (int i = 0; i < m_last_populated;) {
    if (m_moves.at(i).is_capture())
      ++i;
    else {
      this->remove(i);
    }
  }
}

int MoveList::get_size() noexcept { return m_last_populated; }

void MoveList::score_all(const Board& bd) noexcept {
  std::for_each(m_moves.begin(), m_moves.begin() + m_last_populated,
                [&](Move& m) mutable { m.set_heuristic(MoveScorer::score(bd, m)); });
}

void MoveList::score_all(const Board& bd, const TTable& tt) noexcept {
  std::for_each(m_moves.begin(), m_moves.begin() + m_last_populated,
                [&](Move& m) mutable { m.set_heuristic(MoveScorer::score(bd, m, tt)); });
}
}  // namespace BeepBoop
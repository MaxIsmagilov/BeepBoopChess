#pragma once

#include <array>
#include <vector>

#include "eval.hpp"
#include "move.hpp"
#include "transposition.hpp"
#include "utils.hpp"

namespace BeepBoop
{

class MoveList
{
 public:
  MoveList();
  const std::array<Move, 256>& get_all() const noexcept;

  void sort() noexcept;

  void partial_sort(int middle) noexcept;

  void move_killer(int num_best, int num_killer) noexcept;

  const Move& operator[](int index) const noexcept;

  void set_heuristic(int index, int heuristic) noexcept;

  void push_back(const Move& mv) noexcept;

  void push_back(Move&& mv) noexcept;

  void remove(int index) noexcept;

  void remove(std::vector<int> indeces) noexcept;

  void remove_non_captures() noexcept;

  int get_size() noexcept;

  void score_all(const Board& bd) noexcept;

  void score_all(const Board& bd, const TTable& tt) noexcept;

 private:
  std::array<Move, 256> m_moves{Move{}};
  int                   m_last_populated{0};
};

}  // namespace BeepBoop
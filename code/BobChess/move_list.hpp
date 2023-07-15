#pragma once

#include <array>
#include <vector>

#include "move.hpp"
#include "utils.hpp"

namespace BobChess
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

  int get_size() noexcept;

 private:
  std::array<Move, 256> m_moves{Move{}};
  int m_last_populated{0};
};

}  // namespace BobChess
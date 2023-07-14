#pragma once

#include <array>

#include "board.hpp"
#include "move.hpp"

namespace BobChess
{

class BoardStack
{
 public:
  BoardStack(const Board& bd);

  BoardStack() = delete;

  /// @brief gets the top of the board
  /// @return a const reference
  const Board& top() const;

  void pop();

  void move(const Move& move);

 private:
  std::array<Board, 512> m_array;
  int size{0};
};

}  // namespace BobChess
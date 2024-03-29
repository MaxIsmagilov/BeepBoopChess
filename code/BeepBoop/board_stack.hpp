#pragma once

#include <array>

#include "board.hpp"
#include "move.hpp"

namespace BeepBoop
{

class BoardStack
{
 public:
  BoardStack(const Board& bd);

  BoardStack() = delete;

  /// @brief gets the top of the board
  /// @return a const reference
  const Board& top() const;

  /// @brief gets last move made
  /// @return a const reference
  const Move& lastmove() const;

  /// @brief pop a board off the top
  void pop();

  /// @brief excecute a move
  /// @param move
  void move(const Move& move);

  /// @brief make a nullmove (side change)
  void nullmove();

 private:
  std::array<Board, 1024> m_boards;
  std::array<Move, 1024> m_moves;
  int size{0};
};

}  // namespace BeepBoop
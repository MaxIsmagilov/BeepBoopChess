#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <tuple>

#include "board.hpp"
#include "board_stack.hpp"
#include "move.hpp"
#include "move_generator.hpp"
#include "transposition.hpp"
#include "utils.hpp"

namespace BobChess
{

class algorithm
{
 public:
  algorithm() = delete;

  algorithm(const Board& bd, TTable& tt);

  std::tuple<Move, int, int> get_best_move(int depth, std::function<int(Board)> eval);

  int evaluate_move(int depth, std::function<int(Board)> eval);

 private:
  TTable& m_tt;
  BoardStack m_bs;

  std::unique_ptr<int> m_count{};

  int quescence(int depth, int alpha, int beta, int color, std::function<int(Board)> eval);
  int negamax(int depth, int alpha, int beta, int color, std::function<int(Board)> eval);

  /// @brief arbitrarily large number to use in place of infinity
  static constexpr int infinity = 1000000;

  /// @brief checkmate value
  static constexpr int game_over = 100000;

  static constexpr int full_depth = 4;

  static constexpr int LMR_R = 3;
};

}  // namespace BobChess

/*function negamax(node, depth, α, β, color) is
    alphaOrig := α

    (* Transposition Table Lookup; node is the lookup key for ttEntry *)
    ttEntry := transpositionTableLookup(node)
    if ttEntry is valid and ttEntry.depth ≥ depth then
        if ttEntry.flag = EXACT then
            return ttEntry.value
        else if ttEntry.flag = LOWERBOUND then
            α := max(α, ttEntry.value)
        else if ttEntry.flag = UPPERBOUND then
            β := min(β, ttEntry.value)

        if α ≥ β then
            return ttEntry.value

    if depth = 0 or node is a terminal node then
        return color × the heuristic value of node

    childNodes := generateMoves(node)
    childNodes := orderMoves(childNodes)
    value := −∞
    for each child in childNodes do
        value := max(value, −negamax(child, depth − 1, −β, −α, −color))
        α := max(α, value)
        if α ≥ β then
            break

    (* Transposition Table Store; node is the lookup key for ttEntry *)
    ttEntry.value := value
    if value ≤ alphaOrig then
        ttEntry.flag := UPPERBOUND
    else if value ≥ β then
        ttEntry.flag := LOWERBOUND
    else
        ttEntry.flag := EXACT
    ttEntry.depth := depth
    ttEntry.is_valid := true
    transpositionTableStore(node, ttEntry)

    return value

(* Initial call for Player A's root node *)
negamax(rootNode, depth, −∞, +∞, 1)*/

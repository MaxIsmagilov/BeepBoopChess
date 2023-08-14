#include "algorithm_chess.hpp"

#include <iostream>

#include "move_generator.hpp"
#include "move_list.hpp"
#include "timekeeping.hpp"

namespace BobChess
{

Algorithm::Algorithm(Board&& bd, std::function<int(Board)> eval, TTable& table)
    : m_tt{table}, m_bs{bd}, m_eval{eval}, m_count{0}, current_guess{0} {}

std::tuple<int, std::size_t> Algorithm::evaluate_move(int depth) {
  current_depth = depth;
  auto n = current_guess;

  n = negamax(depth, -infinity, infinity);

  /*
  auto upperbound = game_over + 1, lowerbound = -game_over - 1;
  do {
    const auto beta = [&]() -> int {
      if (n == lowerbound)
        return n + 1;
      else
        return n;
    }();
    // auto startcolor = (m_bs.top().side_to_move()) ? 1 : 1;
    n = negamax(depth, beta - 1, beta);
    if (n < beta)
      upperbound = n;
    else
      lowerbound = n;
    // std::cout << (std::to_string(beta) + "\n");
  } while (lowerbound < upperbound);

  current_guess = n;*/

  return std::make_tuple(n, m_count);
}

int Algorithm::quescence(int depth, int alpha, int beta) {
  ++m_count;
  auto&& bd = m_bs.top();

  auto delta = Evaluator::get_piece(utils::QUEEN);
  static constexpr auto delta_change = Evaluator::get_piece(utils::QUEEN) - Evaluator::get_piece(utils::PAWN) * 2;

  if (m_bs.lastmove().is_promote()) delta += delta_change;

  const auto stand_pat = -m_eval(bd);
  if (stand_pat >= beta || depth == 0) return stand_pat;
  if (stand_pat < alpha - delta)
    return alpha;
  else if (alpha < stand_pat)
    alpha = stand_pat;

  auto ml = MoveGenerator::generate_all(bd);

  if (ml.get_size() == 0) {
    if (MoveGenerator::in_check(bd, bd.side_to_move())) {
      return -(game_over - (current_depth - (depth - q_depth)));
    } else {
      return 0;
    }
  }

  ml.remove_non_captures();

  ml.score_all(bd);
  ml.sort();
  ml.move_killer(1, 1);

  for (int i = 0; i < ml.get_size(); ++i) {
    auto score = -quescence(depth - 1, -beta, -alpha);

    if (score >= beta) return beta;
    if (score > alpha) alpha = score;
  }

  return alpha;
}

int Algorithm::negamax(int depth, int alpha, int beta) {
  ++m_count;
  auto&& bd = m_bs.top();

  if (bd.halfmoves() >= 50) return 0;

  if (depth <= 0) {
    return quescence(q_depth, alpha, beta);
  }

  MoveList ml = MoveGenerator::generate_all(bd);

  if (ml.get_size() == 0) {
    if (MoveGenerator::in_check(bd, bd.side_to_move()))
      return -(game_over - (current_depth - depth));
    else
      return 0;
  }

  const auto entry = m_tt.get_entry(bd);
  if (entry.m_is_valid && entry.depth >= depth) {
    if (entry.m_lower >= beta) return entry.m_lower;
    if (entry.m_upper <= alpha) return entry.m_upper;
    alpha = std::max(alpha, entry.m_lower);
    beta = std::min(beta, entry.m_upper);
  }

  auto value = -infinity;

  ml.score_all(bd, m_tt);
  ml.sort();
  ml.move_killer(1, 1);

  if ((current_depth - depth) > full_depth && !MoveGenerator::in_check(bd, bd.side_to_move())) {
    m_bs.nullmove();
    value = -negamax(depth - 4, -beta, -(beta - 1));
    m_bs.pop();
    if (value >= beta) return value;
  }

  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    const int oldval = value;

    if (ml[i].is_reduceable() && (current_depth - depth) > 3)
      value = -negamax(depth - 2, -beta, -alpha);
    else
      value = -negamax(depth - 1, -beta, -alpha);

    m_bs.pop();

    value = std::max(value, oldval);
    alpha = std::max(value, alpha);
    if (alpha >= beta) break;
  }

  TTutils::TTEntry new_entry = {TTable::get_key(bd), 0, 0, 0, TTutils::VALID};

  if (value <= alpha)
    new_entry.m_upper = value;
  else if (value >= beta)
    new_entry.m_lower = value;
  else {
    new_entry.m_lower = value;
    new_entry.m_upper = value;
  }

  m_tt.add(new_entry);
  return value;
}

}  // namespace BobChess

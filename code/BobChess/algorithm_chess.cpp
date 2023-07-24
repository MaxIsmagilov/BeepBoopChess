#include "algorithm_chess.hpp"

#include <iostream>

#include "move_generator.hpp"
#include "move_list.hpp"
#include "timekeeping.hpp"

namespace BobChess
{

Algorithm::Algorithm(Board&& bd, std::function<int(Board)> eval)
    : m_tt(), m_bs{bd}, m_eval{eval}, m_count{0}, current_guess{0} {}

std::tuple<int, std::size_t> Algorithm::evaluate_move(int depth) {
  current_depth = depth;
  auto n = current_guess;

  auto upperbound = game_over + 1, lowerbound = -game_over - 1;
  do {
    const auto beta = [&]() -> int {
      if (n == lowerbound)
        return n + 1;
      else
        return n;
    }();
    // auto startcolor = (m_bs.top().side_to_move()) ? 1 : 1;
    n = -negamax(depth, beta - 1, beta, -1);
    if (n < beta)
      upperbound = n;
    else
      lowerbound = n;
  } while (lowerbound < upperbound);

  current_guess = n;

  return std::make_tuple(n, m_count);
}

int Algorithm::quescence(int depth, int alpha, int beta, int color) {
  ++m_count;
  auto& bd = m_bs.top();

  if (bd.halfmoves() >= 50) {
    return 0;
  }

  MoveList ml = MoveGenerator::generate_all(bd);

  if (ml.get_size() == 0) {
    if (MoveGenerator::in_check(bd, bd.side_to_move())) {
      return -(game_over - (current_depth - (depth - q_depth)));
    } else {
      return 0;
    }
  }

  ml.score_all(bd);

  std::vector<int> not_captures;
  not_captures.reserve(ml.get_size());

  for (int i = 0; i < ml.get_size(); ++i) {
    if (!ml[i].is_capture()) not_captures.emplace_back(i);
  }

  ml.remove(not_captures);

  if (ml.get_size() == 0 || depth <= 0) {
    return m_eval(bd) * color;
  }
  auto alpha_orig = alpha;

  const auto entry = m_tt.get_entry(bd);
  if (entry.m_is_valid && entry.depth >= depth) {
    if (entry.m_lower >= beta) return entry.m_lower;
    if (entry.m_upper <= alpha) return entry.m_upper;
    alpha = std::max(alpha, entry.m_lower);
    beta = std::min(beta, entry.m_upper);
  }

  auto value = -infinity;
  bool first = true;

  ml.sort();
  ml.move_killer(1, 2);

  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    const int oldval = value;
    // if (first) {
    value = -quescence(depth - 1, -beta, -alpha, -color);
    /*  first = false;

    } else {
      value = -quescence(depth - 1, -(alpha + 1), -alpha, -color);
      if (alpha < value && value < beta) value = -quescence(depth - 1, -beta, -value, -color);
    }*/
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

int Algorithm::negamax(int depth, int alpha, int beta, int color) {
  ++m_count;
  auto& bd = m_bs.top();

  if (bd.halfmoves() >= 50) return 0;

  if (depth <= 0) {
    return quescence(q_depth, alpha, beta, color);
  }

  MoveList ml = MoveGenerator::generate_all(bd);

  if (ml.get_size() == 0) {
    if (MoveGenerator::in_check(bd, bd.side_to_move()))
      return -(game_over - (current_depth - depth));
    else
      return 0;
  }

  auto alpha_orig = alpha;

  const auto entry = m_tt.get_entry(bd);
  if (entry.m_is_valid && entry.depth >= depth) {
    if (entry.m_lower >= beta) return entry.m_lower;
    if (entry.m_upper <= alpha) return entry.m_upper;
    alpha = std::max(alpha, entry.m_lower);
    beta = std::min(beta, entry.m_upper);
  }
  auto value = -infinity;
  bool first = true;

  ml.score_all(bd);
  ml.sort();
  ml.move_killer(1, 2);

  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    const int oldval = value;

    value = -negamax(depth - 1, -beta, -alpha, -color);

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

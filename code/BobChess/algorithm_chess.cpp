#include "algorithm_chess.hpp"

#include <iostream>

#include "move_generator.hpp"
#include "move_list.hpp"
#include "timekeeping.hpp"

namespace BobChess
{

Algorithm::Algorithm(Board bd, std::function<int(Board)> eval)
    : m_bs{bd}, m_tt(), m_eval{eval}, m_count{std::make_unique<int>(0)} {}

std::tuple<int, int> Algorithm::evaluate_move(int depth) {
  current_depth = depth;
  auto startcolor = (m_bs.top().side_to_move()) ? 1 : -1;
  const auto n = -negamax(depth, -infinity, infinity, startcolor);
  return std::make_tuple(n, *m_count);
}

int Algorithm::quescence(int depth, int alpha, int beta, int color) {
  ++(*m_count);
  auto& bd = m_bs.top();

  if (bd.halfmoves() >= 50) return 0;

  MoveList ml = MoveGenerator::generate_all(bd);

  if (ml.get_size() == 0) {
    if (MoveGenerator::in_check(bd, bd.side_to_move()))
      return (game_over + depth + 3);
    else
      return 0;
  }

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
  const auto entryvalue = std::get<0>(entry);
  const auto entrytype = std::get<1>(entry);
  const auto entrydepth = std::get<2>(entry);
  if (entrytype != TTutils::FAIL) {
    if (entrytype == TTutils::EXACT)
      return entryvalue;
    else if (entrytype == TTutils::UBOUND)
      alpha = std::max(alpha, entryvalue);
    else if (entrytype == TTutils::LBOUND)
      beta = std::min(beta, entryvalue);

    if (alpha >= beta) return entryvalue;
  }

  auto value = -infinity;
  bool first = true;

  ml.sort();
  ml.move_killer(2, 2);

  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    int newval;
    if (first) {
      newval = -quescence(depth - 1, -beta, -alpha, -color);
      first = false;
    } else {
      newval = -quescence(depth - 1, -(alpha + 1), -alpha, -color);
      if (alpha < value && value < beta) newval = -quescence(depth - 1, -beta, -value, -color);
    }
    m_bs.pop();

    value = std::max(value, newval);
    alpha = std::max(value, alpha);
    if (alpha >= beta) break;
  }

  auto newtype = TTutils::FAIL;
  if (value <= alpha_orig)
    newtype = TTutils::UBOUND;
  else if (value >= beta)
    newtype = TTutils::LBOUND;
  else
    newtype = TTutils::EXACT;

  m_tt.add(bd, value, depth, newtype);

  return value;
}

int Algorithm::negamax(int depth, int alpha, int beta, int color) {
  ++(*m_count);
  // std::string s = std::to_string(*m_count) + " at ";
  // s += std::to_string(depth);
  // s += " here\n";
  // std::cout << s;
  auto& bd = m_bs.top();

  if (bd.halfmoves() >= 50) return 0;

  if (depth <= 0) {
    return quescence(2, alpha, beta, color);
  }

  MoveList ml = MoveGenerator::generate_all(bd);

  if (ml.get_size() == 0) {
    if (MoveGenerator::in_check(bd, bd.side_to_move()))
      return (game_over + depth + 3);
    else
      return 0;
  }
  auto alpha_orig = alpha;

  const auto entry = m_tt.get_entry(bd);
  const auto entryvalue = std::get<0>(entry);
  const auto entrytype = std::get<1>(entry);
  const auto entrydepth = std::get<2>(entry);
  if (entrytype != TTutils::FAIL) {
    if (entrytype == TTutils::EXACT)
      return entryvalue;
    else if (entrytype == TTutils::UBOUND)
      alpha = std::max(alpha, entryvalue);
    else if (entrytype == TTutils::LBOUND)
      beta = std::min(beta, entryvalue);

    if (alpha >= beta) return entryvalue;
  }
  auto value = -infinity;
  bool first = true;

  ml.sort();
  ml.move_killer(2, 2);

  if (current_depth - depth > full_depth) {
    m_bs.nullmove();
    value = -negamax(depth - 4, -beta, -(beta - 1), -color);
    m_bs.pop();
    if (value >= beta) return value;
  }

  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    const int oldval = value;
    if (first) {
      value = -negamax(depth - 1, -beta, -alpha, -color);
      first = false;

    } else {
      if (depth >= LMR_R && ml[i].is_reduceable()) {
        value = -negamax(depth - 2, -(alpha + 1), -alpha, -color);
      } else
        value = alpha + 1;

      if (value > alpha) {
        value = -negamax(depth - 1, -(alpha + 1), -alpha, -color);
        value = std::max(value, value);
        if (alpha < value && value < beta) value = -negamax(depth - 1, -beta, -value, -color);
      }
    }
    m_bs.pop();

    value = std::max(value, oldval);
    alpha = std::max(value, alpha);
    if (alpha >= beta) break;
  }

  auto newtype = TTutils::FAIL;
  if (value <= alpha_orig)
    newtype = TTutils::UBOUND;
  else if (value >= beta)
    newtype = TTutils::LBOUND;
  else
    newtype = TTutils::EXACT;

  m_tt.add(bd, value, depth, newtype);

  return value;
}

}  // namespace BobChess

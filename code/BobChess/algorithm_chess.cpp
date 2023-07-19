#include "algorithm_chess.hpp"

#include <iostream>

#include "move_generator.hpp"
#include "move_list.hpp"
namespace BobChess
{

algorithm::algorithm(const Board& bd, TTable& tt) : m_bs{bd}, m_tt{tt}, m_count{std::make_unique<int>(0)} {}

int algorithm::evaluate_move(int depth, std::function<int(Board)> eval) {
  auto startcolor = (m_bs.top().side_to_move()) ? 1 : -1;
  const auto n = -negamax(depth, -infinity, infinity, startcolor, eval);
  return n;
}

std::tuple<Move, int, int> algorithm::get_best_move(int depth, std::function<int(Board)> eval) {
  MoveList ml = MoveGenerator::generate_all(m_bs.top());
  if (ml.get_size() == 0) return std::make_tuple(Move{}, 0, 0);
  for (int d = std::max(std::min(2, depth), 1); d < depth; ++d) {
    for (int i = 0; i < ml.get_size(); ++i) {
      m_bs.move(ml[i]);
      ml.set_heuristic(i, evaluate_move(d, eval));
      m_bs.pop();
      // std::cout << '\t' << ml[i].to_string() << '\t' << ml[i].get_heuristic() << '\n';
    }
    std::cout << *m_count << "\t<--- at:" << d + 1 << '\n';
  }
  ml.sort();
  return std::make_tuple(ml[0], ml[0].get_heuristic(), *m_count);
}

int algorithm::quescence(int depth, int alpha, int beta, int color, std::function<int(Board)> eval) {
  ++(*m_count);

  MoveList ml = MoveGenerator::generate_all(m_bs.top());

  if (ml.get_size() == 0) {
    return -(game_over + depth);
  }

  std::vector<int> not_captures;
  not_captures.reserve(ml.get_size());

  for (int i = 0; i < ml.get_size(); ++i) {
    if (!ml[i].is_capture()) not_captures.emplace_back(i);
  }

  ml.remove(not_captures);

  if (ml.get_size() == 0 || depth <= 0) {
    return eval(m_bs.top()) * color;
  }
  auto alpha_orig = alpha;

  const auto entry = m_tt.get_entry(m_bs.top());
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
      newval = -quescence(depth - 1, -beta, -alpha, -color, eval);
      first = false;
    } else {
      newval = -quescence(depth - 1, -(alpha + 1), -alpha, -color, eval);
      if (alpha < value && value < beta) newval = -quescence(depth - 1, -beta, -value, -color, eval);
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

  m_tt.add(m_bs.top(), value, depth, newtype);

  return value;
}

int algorithm::negamax(int depth, int alpha, int beta, int color, std::function<int(Board)> eval) {
  if (depth <= 0) {
    return quescence(2, alpha, beta, color, eval);
  }

  ++(*m_count);

  MoveList ml = MoveGenerator::generate_all(m_bs.top());

  if (ml.get_size() == 0) {
    return -(game_over + depth + 3);
  }

  auto alpha_orig = alpha;

  const auto entry = m_tt.get_entry(m_bs.top());
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

  if (true) {
    m_bs.nullmove();
    value = -negamax(depth - 4, -beta, -(beta - 1), -color, eval);
    m_bs.pop();
    if (value >= beta) return value;
  }

  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    const int oldval = value;
    if (first) {
      value = -negamax(depth - 1, -beta, -alpha, -color, eval);
      first = false;

    } else {
      if (depth >= LMR_R && ml[i].is_reduceable()) {
        value = -negamax(depth - 2, -(alpha + 1), -alpha, -color, eval);
      } else
        value = alpha + 1;

      if (value > alpha) {
        value = -negamax(depth - 1, -(alpha + 1), -alpha, -color, eval);
        value = std::max(value, value);
        if (alpha < value && value < beta) value = -negamax(depth - 1, -beta, -value, -color, eval);
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

  m_tt.add(m_bs.top(), value, depth, newtype);

  return value;
}

}  // namespace BobChess

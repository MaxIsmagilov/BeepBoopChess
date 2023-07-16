#include "algorithm_chess.hpp"

#include <iostream>

#include "move_generator.hpp"
#include "move_list.hpp"
namespace BobChess
{

algorithm::algorithm(const Board& bd, TTable& tt) : m_bs{bd}, m_tt{tt}, m_count{std::make_unique<int>(0)} {}

int algorithm::evaluate_move(int depth, std::function<int(Board)> eval) {
  const auto n = negamax(depth, -infinity, infinity, 1, eval);
  return n;
}

std::tuple<Move, int, int> algorithm::get_best_move(int depth, std::function<int(Board)> eval) {
  MoveList ml = MoveGenerator::generate_all(m_bs.top());
  if (ml.get_size() == 0) return std::make_tuple(Move{}, 0, 0);
  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    ml.set_heuristic(i, evaluate_move(depth - 1, eval));
    m_bs.pop();
    std::cout << '\t' << ml[i].to_string() << '\t' << ml[i].get_heuristic() << '\n';
  }
  ml.sort();
  return std::make_tuple(ml[0], ml[0].get_heuristic(), *m_count);
}

int algorithm::quescence(int depth, int alpha, int beta, int color, std::function<int(Board)> eval) { return 0; }

int algorithm::negamax(int depth, int alpha, int beta, int color, std::function<int(Board)> eval) {
  ++(*m_count);

  auto alpha_orig = alpha;

  MoveList ml = MoveGenerator::generate_all(m_bs.top());

  if (ml.get_size() == 0 || depth <= 0) {
    return eval(m_bs.top()) * -color;
  }

  const auto entry = m_tt.get_entry(m_bs.top());
  const auto entrytype = std::get<1>(entry);
  const auto entryvalue = std::get<0>(entry);
  if (entrytype != TTutils::FAIL) {
    if (entrytype == TTutils::EXACT) return entryvalue;
    if (entrytype == TTutils::LBOUND) alpha = std::max(alpha, entryvalue);
    if (entrytype == TTutils::UBOUND) beta = std::min(beta, entryvalue);
  }

  auto value = -infinity;
  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    const auto newval = -negamax(depth - 1, -beta, -alpha, -color, eval);
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

  m_tt.add(m_bs.top(), value, newtype);

  return value;
}

}  // namespace BobChess

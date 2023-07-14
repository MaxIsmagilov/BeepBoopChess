#include "algorithm.hpp"

#include "move_generator.hpp"
#include "move_list.hpp"

namespace BobChess
{

template <evaluator the_eval>
algorithm<the_eval>::algorithm(const Board& bd, TTable& tt)
    : m_bs(bd), m_tt{tt}, m_node_count{std::make_shared<u64>()} {}

template <evaluator the_eval>
std::tuple<const Move&, u64, int> algorithm<the_eval>::get_best_move(int depth) {
  MoveList ml = MoveGenerator::generate_all(m_bs.top());
  if (ml.get_size() == 0) return std::make_tuple(Move{}, 0, 0);
  for (int i = 0; i < ml.get_size(); ++i) {
    m_bs.move(ml[i]);
    ml.set_heuristic(i, negamax(depth, negamax(depth, -infinity, infinity, 1)));
    m_bs.pop();
  }
  return std::make_tuple(ml[0], 0, 0);
}

template <evaluator the_eval>
int algorithm<the_eval>::quescence(int depth, int alpha, int beta, int color) {
  auto alpha_orig = alpha;

  MoveList ml = MoveGenerator::generate_all(m_bs.top());

  if (ml.get_size() == 0 || depth == 0) return the_eval::eval(m_bs.top()) * color;

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
    const auto newval = -negamax(depth - 1, -beta, -alpha, -color);
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

template <evaluator the_eval>
int algorithm<the_eval>::negamax(int depth, int alpha, int beta, int color) {
  return 0;
}

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

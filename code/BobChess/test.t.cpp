#include <chrono>
#include <iostream>
#include <string>

#include "algorithm_chess.hpp"
#include "move_generator.hpp"
#include "move_list.hpp"
#include "pesto_eval.hpp"
#include "transposition.hpp"

using namespace BobChess;
using namespace std::chrono;

int perft(const Board& bd, int depth) {
  if (depth == 0) return 1;
  MoveList m = MoveGenerator::generate_all(bd);
  int nodes{0};
  for (int i = 0; i < m.get_size(); ++i) {
    Board bnew = bd;
    bnew.make_move(m[i]);
    nodes += perft(bnew, depth - 1);
  }
  return nodes;
}

int main() {
  std::cout << "start\n";
  TTable::initialize();
  Evaluator::initialize_evaluation();
  TTable t;
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  algorithm a(bd, t);

  auto z = a.get_best_move(14, Evaluator::eval);

  std::cout << std::get<0>(z).to_string() << '\n';
  std::cout << std::get<1>(z) << '\n';
  std::cout << std::get<2>(z) << '\n';

  std::cout << "done\n";

  return 0;
}

#include <array>
#include <bit>
#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "algorithm_chess.hpp"
#include "eval.hpp"
#include "move_finder.hpp"
#include "move_generator.hpp"
#include "move_list.hpp"
#include "timekeeping.hpp"
#include "transposition.hpp"

using namespace BobChess;
using namespace std::chrono;

int perft(const Board& bd, int depth) {
  if (depth == 0) return 1;
  MoveList m = MoveGenerator::generate_all(bd);
  if (m.get_size() == 0) return 1;
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
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("r3r1k1/pppb1Npp/2np4/4b3/4P1nq/2N1B3/QPP2PPP/R3R1K1 w p - 0 1");
  // bd.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  // bd.import_FEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");

  for (int i = 0; i < 5; ++i) {
    std::cout << bd.nice_print() << '\n';

    Clock c;
    c.start();

    auto t = MoveFinder::get_best_move_time(bd, 5000, Evaluator::eval);

    auto el = c.elapsed_ms();

    auto m = dynamic_cast<Move&>(std::get<0>(t));

    std::cout << m.to_string() << '\n';
    if (m.get_start() == m.get_end()) break;
    std::cout << m.get_heuristic() / (100.0) << '\n';
    std::cout << std::get<1>(t) << " nodes @ ";
    std::cout << std::get<1>(t) / (el) << " k nodes/s\n";

    bd.make_move(m);
  }
  std::cout << '\n' << bd.nice_print();
  std::cout << "done\n";

  return 0;
}

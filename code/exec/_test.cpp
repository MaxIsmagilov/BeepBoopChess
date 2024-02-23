#include <array>
#include <bit>
#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "algorithm_chess.hpp"
#include "eval.hpp"
#include "game.hpp"
#include "move_finder.hpp"
#include "move_generator.hpp"
#include "move_list.hpp"
#include "timekeeping.hpp"
#include "transposition.hpp"

using namespace BeepBoop;
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

  Game g;
  g.set_pos();
  g.set_pos("r3r1k1/pppb1Npp/2np4/4b3/4P1nq/2N1B3/QPP2PPP/R3R1K1 w p - 0 1");
  //  g.set_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");

  TTable ttt;

  for (int i = 0; i < 1; ++i) {
    g.print();

    Clock c;
    c.start();

    auto t = g.go_depth(8, 8);

    auto el = c.elapsed_ms();

    auto m = dynamic_cast<Move&>(std::get<0>(t));

    std::cout << m.to_string() << '\n';
    if (m.get_start() == m.get_end()) break;
    std::cout << m.get_heuristic() / (100.0) << '\n';
    std::cout << std::get<1>(t) << " nodes @ ";
    std::cout << std::get<1>(t) / (el) << " k nodes/s\n";
  }

  g.print();

  std::cout << "done\n";

  return 0;
}

#include <iostream>
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

int perft(const Board& bd, int depth, bool verbose = false) {
  int nodes{0};
  if (depth == 0) return 1;
  MoveList m = MoveGenerator::generate_all(bd);
  for (int i = 0; i < m.get_size(); ++i) {
    Board bnew = bd;
    bnew.make_move(m[i]);
    auto n = perft(bnew, depth - 1, verbose);
    // if (verbose && m[i].is_capture()) [[unlikely]]
    //   std::cerr << bd.debug_print() << "\tmove:" << m[i].to_string() << "\n\n";
    nodes += n;
  }
  return nodes;
}

int main() {
  MoveGenerator::initialize_all();

  // std::string s = "r3k2r/p1ppqpb1/bn1Ppnp1/4N3/1p2P3/2N2Q2/PPPBBPpP/R3KR2 b Qkq - 1 2";
  std::string s = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0";
  Board       b;
  b.import_FEN(s.c_str());
  do {
    std::cout << b.debug_print() << '\n';
    int depth, sum{0};
    std::cout << "depth?\n";
    std::cin >> depth;
    if (depth == 0) return 0;
    MoveList m = MoveGenerator::generate_all(b);
    std::cout << "###### " << m.get_size() << " ######\n";
    for (int i = 0; i < m.get_size(); ++i) {
      std::cout << m[i].to_string() << " ";
      Board newb = b;
      newb.make_move(m[i]);
      auto n = perft(newb, depth - 1);
      std::cout << n << '\t' << i << '\n';
      sum += n;
    }
    int index = 0;
    std::cout << '\n' << sum << '\n';
    std::cout << "zoom?\n";
    std::cin >> index;
    if (index == -1) break;
    b.make_move(m[index]);
  } while (true);
  return 0;
}
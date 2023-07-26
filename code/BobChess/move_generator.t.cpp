#include "move_generator.hpp"

#include <gtest/gtest.h>

#include <string>

using namespace BobChess;

TEST(Generator, Opening) {
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  MoveList m = MoveGenerator::generate_all(bd);
  EXPECT_EQ(20, m.get_size());
}

TEST(Generator, Castleable) {
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("rnbqkbnr/pp3ppp/2ppp3/8/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");
  MoveList m = MoveGenerator::generate_all(bd);
  EXPECT_EQ(33, m.get_size());
}

int castle{0};

int perft(const Board& bd, int depth, bool verbose = false) {
  if (depth == 0) return 1;
  MoveList m = MoveGenerator::generate_all(bd);
  int nodes{0};
  for (int i = 0; i < m.get_size(); ++i) {
    Board bnew = bd;
    bnew.make_move(m[i]);
    if (m[i].is_castle()) ++::castle;
    auto n = perft(bnew, depth - 1, verbose);
    if (verbose) [[unlikely]]
      std::cerr << depth << '\t' << m[i].to_string() << "\t --> " << n << '\n';
    nodes += n;
  }
  return nodes;
}

TEST(Generator, PerftKiwipete) {
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");
  int results[6] = {1, 48, 2039, 97862, 4085603};
  for (int i = 0; i < 4; ++i) {
    ::castle = 0;
    int test = perft(bd, i);
    std::cerr << ::castle << '\n';
    ASSERT_EQ(test, results[i]);
  }
}

TEST(Generator, PerftStart) {
  ::castle = 0;
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  int results[6] = {1, 20, 400, 8902, 197281, 4865609};
  for (int i = 0; i < 6; ++i) {
    ::castle = 0;
    int test = perft(bd, i);
    std::cerr << ::castle << '\n';
    ASSERT_EQ(test, results[i]);
  }
}

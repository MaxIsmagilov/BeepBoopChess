#include <gtest/gtest.h>

#include <string>

#include "move_generator.hpp"

using namespace BeepBoop;

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
int checkmates{0};
int promotions{0};
int enpassant{0};
int capture{0};

int perft(const Board& bd, int depth, bool verbose = false) {
  int nodes{0};
  if (depth == 0) return 1;
  MoveList m = MoveGenerator::generate_all(bd);
  if (MoveGenerator::in_check(bd, bd.side_to_move()) && !m.get_size()) ++::checkmates;
  for (int i = 0; i < m.get_size(); ++i) {
    Board bnew = bd;
    bnew.make_move(m[i]);
    if (m[i].is_promote()) ++::promotions;
    if (m[i].is_castle()) ++::castle;
    if (m[i].is_enpassant()) ++::enpassant;
    if (m[i].is_capture()) ++::capture;
    auto n = perft(bnew, depth - 1, verbose);
    // if (verbose && m[i].is_capture()) [[unlikely]]
    //   std::cerr << bd.debug_print() << "\tmove:" << m[i].to_string() << "\n\n";
    nodes += n;
  }
  return nodes;
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

TEST(Generator, PerftKiwipete) {
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");
  int results[6] = {1, 48, 2039, 97862, 4085603};
  struct r
  {
    int a, b, c, d;
  };

  r leaves[6] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int i = 1; i < 5; ++i) {
    ::capture    = 0;
    ::promotions = 0;
    ::castle     = 0;
    ::enpassant  = 0;
    int test     = perft(bd, i);
    leaves[i].a  = ::capture;
    leaves[i].b  = ::promotions;
    leaves[i].c  = ::castle;
    leaves[i].d  = ::enpassant;
    std::cerr << leaves[i].a - leaves[i - 1].a << '\t';
    std::cerr << leaves[i].b - leaves[i - 1].b << '\t';
    std::cerr << leaves[i].c - leaves[i - 1].c << '\t';
    std::cerr << leaves[i].d - leaves[i - 1].d << '\n';
    std::cerr << "\t" << test - results[i] << '\n';
    ASSERT_EQ(test, results[i]);
  }
}

TEST(Generator, PerftCPW3) {
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0");
  int results[6] = {1, 14, 191, 2812, 43238};
  for (int i = 0; i < 5; ++i) {
    ::castle = 0;
    int test = perft(bd, i, true);
    std::cerr << ::castle << '\n';
    ASSERT_EQ(test, results[i]);
  }
}

TEST(Generator, PerftCPW4) {
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  int results[6] = {1, 6, 264, 9467, 422333, 15833292};
  for (int i = 0; i < 6; ++i) {
    ::castle = 0;
    int test = perft(bd, i, true);
    std::cerr << ::castle << '\n';
    ASSERT_EQ(test, results[i]);
  }
}

TEST(Generator, PerftCPW5) {
  MoveGenerator::initialize_all();
  Board bd;
  bd.import_FEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  int results[6] = {1, 44, 1486, 62379, 2103487};
  for (int i = 0; i < 4; ++i) {
    ::castle = 0;
    int test = perft(bd, i, true);
    std::cerr << ::castle << '\n';
    ASSERT_EQ(test, results[i]);
  }
}

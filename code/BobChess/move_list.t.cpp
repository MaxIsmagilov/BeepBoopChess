#include "move_list.hpp"

#include <gtest/gtest.h>

using namespace BobChess;

TEST(MoveList, Remove) {
  MoveList m;
  m.push_back(Move());
  m.push_back(Move());
  m.push_back(Move());
  m.set_heuristic(0, 0);
  m.set_heuristic(1, 1);
  m.set_heuristic(2, 2);
  int size = m.get_size();
  EXPECT_EQ(size, 3);
  m.remove(0);
  EXPECT_EQ(size - 1, m.get_size());

  EXPECT_EQ(m[0].get_heuristic(), 1);
  EXPECT_EQ(m[1].get_heuristic(), 2);
}

TEST(MoveList, SortAndOrder) {
  MoveList m;
  m.push_back(Move());
  m.push_back(Move());
  m.push_back(Move());
  m.push_back(Move());
  m.push_back(Move());
  m.set_heuristic(0, 0);
  m.set_heuristic(1, 15);
  m.set_heuristic(2, 2);
  m.set_heuristic(3, 3);
  m.set_heuristic(4, 12);
  int size = m.get_size();
  EXPECT_EQ(size, 5);

  m.sort();

  EXPECT_EQ(m[0].get_heuristic(), 15);
  EXPECT_EQ(m[1].get_heuristic(), 12);
  EXPECT_EQ(m[2].get_heuristic(), 3);
  EXPECT_EQ(m[3].get_heuristic(), 2);
  EXPECT_EQ(m[4].get_heuristic(), 0);

  m.move_killer(1, 1);

  EXPECT_EQ(m[0].get_heuristic(), 15);
  EXPECT_EQ(m[1].get_heuristic(), 0);
  EXPECT_EQ(m[2].get_heuristic(), 12);
  EXPECT_EQ(m[3].get_heuristic(), 3);
  EXPECT_EQ(m[4].get_heuristic(), 2);
}
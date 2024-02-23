#include <gtest/gtest.h>

#include "utils.hpp"

struct kv
{
  BeepBoop::u64 num;
  int           count;
} tests[6] = {{0x1ULL, 1}, {0xFULL, 4}, {0x0ULL, 0}, {0xF0000ULL, 4}, {0x3ULL, 2}, {0x5000ULL, 2}};

TEST(Utils, Count) {
  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(BeepBoop::utils::count_bits(tests[i].num), tests[i].count);
  }
}
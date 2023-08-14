#pragma once

#include <cstdlib>

#include "utils.hpp"

namespace BeepBoop
{

class RandomGenerator
{
 public:
  u32 get_random();

  u64 random_u64();

  u64 magic_candidate();

 private:
  u32 random_state = 0xC6497B5D;
};

}  // namespace BeepBoop
#include <cstdlib>

#include "random_generator.hpp"

namespace BeepBoop
{

u32 RandomGenerator::get_random() {
  u32 number = random_state;

  number ^= number >> 13;
  number ^= number << 17;
  number ^= number >> 5;

  random_state = number;

  return number;
}

u64 RandomGenerator::random_u64() {
  u64 n1 = get_random() & 0xFFFF, n2 = get_random() & 0xFFFF, n3 = get_random() & 0xFFFF, n4 = get_random() & 0xFFFF;
  return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

u64 RandomGenerator::magic_candidate() { return random_u64() & random_u64() & random_u64(); }

}  // namespace BeepBoop
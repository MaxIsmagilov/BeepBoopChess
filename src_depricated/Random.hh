#ifndef randomgen
#define randomgen

#include <bits/stdc++.h>

#include <cstdlib>

namespace BCdepricated
{
unsigned int random_state = 0xC6497B5D;

static inline unsigned int random() {
  unsigned int number = random_state;

  number ^= number >> 13;
  number ^= number << 17;
  number ^= number >> 5;

  random_state = number;

  return number;
}

uint64_t random_uint64() {
  uint64_t num;

  uint64_t n1 = random() & 0xFFFF, n2 = random() & 0xFFFF, n3 = random() & 0xFFFF, n4 = random() & 0xFFFF;

  return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

uint64_t magic_candidate() { return random_uint64() & random_uint64() & random_uint64(); }
}  // namespace BCdepricated
#endif

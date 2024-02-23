#include "utils.hpp"

namespace BeepBoop::utils
{

std::string print_bitboard(const u64& bb) {
  std::string str = "";
  for (int i = 0; i < 64; i++) {
    str += (get_bit(bb, i)) ? '1' : '0';
    if (i % 8 == 7) {
      str += '\n';
    }
  }
  return str;
}

unsigned int count_bits(const u64& bitboard) {
  u64          test      = bitboard;
  unsigned int bitnumber = 0;
  while (test) {
    test &= test - 1;
    bitnumber++;
  }
  return bitnumber;
}

int LSB_index(const u64& test) { return (test) ? count_bits((test & -test) - 1) : -1; }

}  // namespace BeepBoop::utils

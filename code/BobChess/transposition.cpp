#include "transposition.hpp"

#include <algorithm>

#include "random_generator.hpp"
#include "utils.hpp"

namespace BobChess
{

TTable::TTable() {
  TTutils::TTEntry null_entry = {0ULL, 0, TTutils::FAIL};
  std::fill(m_entries.begin(), m_entries.end(), null_entry);
}

u64 TTable::get_key(const Board& bd) noexcept {
  u64 key = 0ULL;
  for (int i = 0; i < 12; ++i) {
    auto bb = bd[12];
    while (bb) {
      auto j = utils::LSB_index(bb);
      bb = utils::set_bit_false(bb, j);
      key ^= m_keygens[j][i];
    }
  }
  return key;
}

std::tuple<int, TTutils::TTEntryType> TTable::get_entry(u64 key) const noexcept {
  key %= TTutils::ttsize;
  auto entry = m_entries[key];
  return std::make_tuple(entry.m_value, entry.m_type);
}

std::tuple<int, TTutils::TTEntryType> TTable::get_entry(const Board& bd) const noexcept {
  auto key = get_key(bd) % TTutils::ttsize;
  auto entry = m_entries[key];
  return std::make_tuple(entry.m_value, entry.m_type);
}

void TTable::add(const Board& bd, int value, TTutils::TTEntryType type) noexcept {
  auto key = get_key(bd);
  m_entries[key % TTutils::ttsize] = {key, value, type};
}

void TTable::initialize() noexcept {
  RandomGenerator r;
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j < 12; ++j) {
      m_keygens[i][j] = r.random_u64();
    }
  }
}

u64 TTable::m_keygens[64][12] = {};

}  // namespace BobChess

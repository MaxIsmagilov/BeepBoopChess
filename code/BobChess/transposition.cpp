#include "transposition.hpp"

#include <algorithm>
#include <iostream>
#include <random>

#include "random_generator.hpp"
#include "utils.hpp"

namespace BobChess
{

TTable::TTable() {
  TTutils::TTEntry null_entry = {0ULL, 0, 0, TTutils::FAIL};
  std::fill(m_entries.begin(), m_entries.end(), null_entry);
}

u64 TTable::get_key(const Board& bd) noexcept {
  u64 key = 0ULL;
  for (int i = 0; i < 12; ++i) {
    auto bb = bd[i];
    while (bb) {
      auto j = utils::LSB_index(bb);
      bb = utils::set_bit_false(bb, j);
      key ^= m_keygens[j][i];
    }
  }
  key ^= m_keygens[(bd.enpassant_square() == 255) ? 0 : bd.enpassant_square()][(bd.enpassant_square() == 255)];
  return key;
}

std::tuple<int, TTutils::TTEntryType, int> TTable::get_entry(u64 key) const noexcept {
  key %= TTutils::ttsize;
  auto entry = m_entries[key];
  if (entry.m_key != key) return std::make_tuple(entry.m_value, TTutils::FAIL, 0);
  return std::make_tuple(entry.m_value, entry.m_type, entry.depth);
}

std::tuple<int, TTutils::TTEntryType, int> TTable::get_entry(const Board& bd) const noexcept {
  auto key = get_key(bd);
  auto entry = m_entries[key % TTutils::ttsize];
  if (entry.m_key != key) return std::make_tuple(entry.m_value, TTutils::FAIL, 0);
  return std::make_tuple(entry.m_value, entry.m_type, entry.depth);
}

void TTable::add(const Board& bd, int value, unsigned char depth, TTutils::TTEntryType type) noexcept {
  auto key = get_key(bd);
  m_entries[key % TTutils::ttsize] = {key, value, depth, type};
}

void TTable::initialize() noexcept {
  std::random_device r;
  std::uniform_int_distribution<u64> dist(0ULL, 0xFFFFFFFFFFFFFFFFULL);
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j < 12; ++j) {
      m_keygens[i][j] = dist(r) ^ dist(r);
    }
  }
}

u64 TTable::m_keygens[64][12] = {};

}  // namespace BobChess

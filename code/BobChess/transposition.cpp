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
  m_entries.resize(TTutils::ttsize);
  std::fill(m_entries.begin(), m_entries.begin() + TTutils::ttsize, null_entry);
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
  auto ep = bd.enpassant_square() + 1;
  if (ep == 256) ep = 0;
  key ^= m_keymods[ep];
  if (bd.castle_available(0)) key ^= m_keymods[65];
  if (bd.castle_available(1)) key ^= m_keymods[66];
  if (bd.castle_available(2)) key ^= m_keymods[67];
  if (bd.castle_available(3)) key ^= m_keymods[68];
  if (bd.side_to_move()) key ^= m_keymods[69];
  return key;
}

TTutils::TTEntry TTable::get_entry(u64 key) const noexcept {
  auto entry = m_entries[key % TTutils::ttsize];
  if (entry.m_key != key)
    return TTutils::FAILED_ENTRY;
  else
    return entry;
}

TTutils::TTEntry TTable::get_entry(const Board& bd) const noexcept {
  const auto key = get_key(bd);
  auto entry = m_entries[key % TTutils::ttsize];
  if (entry.m_key != key)
    return TTutils::FAILED_ENTRY;
  else
    return entry;
}

void TTable::add(TTutils::TTEntry entry) noexcept { m_entries[entry.m_key % TTutils::ttsize] = entry; }

void TTable::initialize() noexcept {
  std::random_device r;
  std::uniform_int_distribution<u64> dist(0ULL, 0xFFFFFFFFFFFFFFFFULL);
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j < 12; ++j) {
      m_keygens[i][j] = dist(r);
    }
  }
  for (int k = 0; k < 70; ++k) {
    m_keymods[k] = dist(r);
  }
}

u64 TTable::m_keygens[64][12] = {};
u64 TTable::m_keymods[70] = {};

}  // namespace BobChess

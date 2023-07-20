#pragma once

#include <array>
#include <tuple>

#include "board.hpp"
#include "utils.hpp"

namespace BobChess
{

namespace TTutils
{

constexpr std::size_t ttsize = (65536 * 4);

// @brief entry type enum
enum TTEntryType : unsigned char { EXACT, LBOUND, UBOUND, FAIL };

/// @brief transposition table structure
struct TTEntry
{
  u64 m_key{0ULL};
  int m_value{0};
  unsigned char depth{0};
  TTEntryType m_type{FAIL};
};

}  // namespace TTutils

class TTable
{
 public:
  TTable();

  static u64 get_key(const Board& bd) noexcept;

  std::tuple<int, TTutils::TTEntryType, int> get_entry(u64 key) const noexcept;

  std::tuple<int, TTutils::TTEntryType, int> get_entry(const Board& bd) const noexcept;

  void add(const Board& bd, int value, unsigned char depth, TTutils::TTEntryType type) noexcept;

  static void initialize() noexcept;

 private:
  static u64 m_keygens[64][12];
  static u64 m_keymods[70];
  std::array<TTutils::TTEntry, TTutils::ttsize> m_entries;
};

}  // namespace BobChess
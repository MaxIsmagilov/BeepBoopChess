#pragma once

#include <tuple>
#include <vector>

#include "board.hpp"
#include "utils.hpp"

namespace BobChess
{

namespace TTutils
{

constexpr std::size_t ttsize = (2097152);

// @brief entry type enum
enum TTEntryType : bool { FAIL, VALID };

/// @brief transposition table structure
struct TTEntry
{
  u64 m_key{0ULL};
  int m_lower{0};
  int m_upper{0};
  int depth{0};
  TTEntryType m_is_valid{FAIL};
};

constexpr TTEntry FAILED_ENTRY = {0, 0, 0, 0, FAIL};

}  // namespace TTutils

class TTable
{
 public:
  TTable();

  static u64 get_key(const Board& bd) noexcept;

  /// @brief
  /// @param key
  /// @return
  TTutils::TTEntry get_entry(u64 key) const noexcept;

  TTutils::TTEntry get_entry(const Board& bd) const noexcept;

  void add(TTutils::TTEntry entry) noexcept;

  static void initialize() noexcept;

 private:
  static u64 m_keygens[64][12];
  static u64 m_keymods[70];
  std::vector<TTutils::TTEntry> m_entries;
};

}  // namespace BobChess
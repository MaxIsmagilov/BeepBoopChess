#pragma once

#include <mutex>
#include <tuple>
#include <vector>

#include "board.hpp"
#include "utils.hpp"

namespace BeepBoop
{

namespace TTutils
{

constexpr std::size_t ttsize = (2097152);

// @brief entry type enum
enum TTEntryType : bool { FAIL, VALID };

/// @brief transposition table structure, includes reference count for ABDADA
struct TTEntry
{
  u64         m_key{0ULL};
  int         m_lower{0};
  int         m_upper{0};
  int         depth{0};
  u8          refcount   : 8 {0};
  TTEntryType m_is_valid : 8 {FAIL};
};

constexpr TTEntry FAILED_ENTRY = {0, 0, 0, 0, FAIL};

}  // namespace TTutils

class TTable
{
 public:
  TTable();

  TTable(TTable&&) = delete;

  static u64 get_key(const Board& bd) noexcept;

  u8 get_visited(u64 key) const noexcept;

  void visit(u64 key) noexcept;

  void leave(u64 key) noexcept;

  /// @brief
  /// @param key
  /// @return
  TTutils::TTEntry get_entry(u64 key) const noexcept;

  TTutils::TTEntry get_entry(const Board& bd) const noexcept;

  void add(TTutils::TTEntry entry) noexcept;

  void combine(std::vector<const TTable*> tables);

  void set_as(TTable* table);

  static void initialize() noexcept;

 private:
  mutable std::mutex                    m_lock{};
  static u64                            m_keygens[64][12];
  static u64                            m_keymods[70];
  mutable std::vector<TTutils::TTEntry> m_entries;
};

}  // namespace BeepBoop
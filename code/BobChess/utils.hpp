#pragma once

// C++ standard library:
#include <string>

// BobChess headers:

namespace BobChess
{

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

namespace utils
{

/// @brief piece enums
enum piece_numbers : u8 { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

/// @brief color enums
enum colors { BLACK, WHITE };

/// @brief squares of pieces
enum squares {
  _A8,
  _B8,
  _C8,
  _D8,
  _E8,
  _F8,
  _G8,
  _H8,
  _A7,
  _B7,
  _C7,
  _D7,
  _E7,
  _F7,
  _G7,
  _H7,
  _A6,
  _B6,
  _C6,
  _D6,
  _E6,
  _F6,
  _G6,
  _H6,
  _A5,
  _B5,
  _C5,
  _D5,
  _E5,
  _F5,
  _G5,
  _H5,
  _A4,
  _B4,
  _C4,
  _D4,
  _E4,
  _F4,
  _G4,
  _H4,
  _A3,
  _B3,
  _C3,
  _D3,
  _E3,
  _F3,
  _G3,
  _H3,
  _A2,
  _B2,
  _C2,
  _D2,
  _E2,
  _F2,
  _G2,
  _H2,
  _A1,
  _B1,
  _C1,
  _D1,
  _E1,
  _F1,
  _G1,
  _H1,
};

constexpr bool get_bit(const u64 bb, int index) { return bb & (1ULL << index); }

constexpr u64 set_bit_true(u64 bb, int index) { return bb | (1ULL << index); }

constexpr u64 set_bit_false(u64 bb, int index) { return bb & ~(1ULL << index); }

/// @brief prints a bitboard
/// @param bb
/// @return a string
std::string print_bitboard(const u64& bb);

/// @brief counts the bits in a bitboard
/// @param bitboard
/// @return the number of bits
unsigned int count_bits(const u64& bitboard);

/// @brief  finds the index of the least significant bit
/// @param test
/// @return the index
int LSB_index(const u64& test);

}  // namespace utils

}  // namespace BobChess
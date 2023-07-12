#pragma once

// C++ standard library:

// BobChess headers:

namespace BobChess {

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned char u8;

namespace utlis {

/// @brief piece enums
enum piece_numbers { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

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

}  // namespace utlis

}  // namespace BobChess
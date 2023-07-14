
#ifndef CHESS_UTILS
#define CHESS_UTILS

#include <bits/stdc++.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <mutex>
#include <numeric>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "MagicNumbers.hh"
#include "Random.hh"

using namespace std::chrono;

namespace BCdepricated
{
/*************************************\
|           TOOL DEFINITIONS          |
\*************************************/

/*      misc. constants, enums, and macroes*/

/// @brief the bits for the a-file
const uint64_t A_FILE = 72340172838076673ULL;

/// @brief the bits for the b-file
const uint64_t B_FILE = 144680345676153346ULL;

/// @brief the bits for the g-file
const uint64_t G_FILE = 4629771061636907072ULL;

/// @brief the bits for the h-file
const uint64_t H_FILE = 9259542123273814144ULL;

/// @brief gamestates (unused)
enum GAMESTATE { RUNNING, BLACK_CHECKMATE, STALEMATE, WHITE_CHECKMATE };

/// @brief piece enums
enum piece_numbers { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

/// @brief color enums
enum colors { WHITE, BLACK };

/// @brief squares of pieces
enum square {
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

/// @brief utility enum for magics (mROOK, mBISHOP)
enum rook_or_bishop { mROOK, mBISHOP };

/// @brief adds an element to an array
#define arradd(a, b, c) \
  a[b] = c;             \
  b++;

/// @brief size of arrays used for move storage
static constexpr unsigned int ARRAY_SIZE = 120;

/*      non-slider attack masks     */

/// @brief pawn attack table
/// @note 0 is black
uint64_t pawn_attacks[2][64];
/// @brief knight attack table
uint64_t knight_attacks[64];
/// @brief king attack table
uint64_t king_attacks[64];

/*       pawn silent moves          */

/// @brief pawn silent move table
/// @note 0 is black
uint64_t silent_pawn_moves[2][64];  //

/*      occupancy masks             */

/// @brief occupancy masks for bishops
uint64_t bishop_attack_masks[64];
/// @brief occupancy masks for rooks
uint64_t rook_attack_masks[64];

/*      magic attack tables         */

/// @brief bishop attack table
uint64_t magic_bishop_attacks[64][512];
/// @brief rook attack table
uint64_t magic_rook_attacks[64][4096];

/*      relevant occupancy bits     */

/// @brief occupancy bits for a square for a bishop
static constexpr int bishop_occupancy_bits[64] = {6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7,
                                                  5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7,
                                                  7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};
/// @brief occupancy bits for a square for a rook
static constexpr int rook_occupancy_bits[64] = {12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11,
                                                11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
                                                11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
                                                11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

/*      wrapper struct to store and sort moves      */

/// @brief struct to store moves and their heuristic
struct move_wrapper
{
  uint32_t _mv;
  u16 _hv;

  explicit move_wrapper() : _mv{0}, _hv{0} {};
  move_wrapper(const uint32_t m, const uint16_t v) : _mv{m}, _hv{v} {};

  inline bool operator<(const move_wrapper& other) const { return this->_hv < other._hv; }
  inline bool operator>(const move_wrapper& other) const { return this->_hv > other._hv; }
};

/*      move encoding guide (using uint32_t wrapper struct)         */

// 0000 0000 0000 0000 0000 0000 0011 1111  start square
// 0000 0000 0000 0000 0000 1111 1100 0000  end square
// 0000 0000 0000 0000 1111 0000 0000 0000  moved piece (0 for none)
// 0000 0000 0000 1111 0000 0000 0000 0000  promotion piece (0 for none)
// 0000 0000 0001 0000 0000 0000 0000 0000  capture flag
// 0000 0000 0010 0000 0000 0000 0000 0000  castling flag
// 0000 0000 0100 0000 0000 0000 0000 0000  enpassant flag
// 0000 0000 1000 0000 0000 0000 0000 0000  double pawn push flag
// 0000 1111 0000 0000 0000 0000 0000 0000  castle overrides (1111 by default, 0 where no longer available)
// 0000 0000 0000 0000 0000 0000 0000 0000  ordering priority (1000 000 = best, 0100 000 = killer, rest are other info)

/*      uint32_t move bit unpacking macroes         */

#define set_heuristic(mv, new_heuristic) (mv._hv = new_heuristic)

#define start_square(mv)                 ((mv._mv) & 0x3FUL)

#define end_square(mv)                   ((mv._mv >> 6) & 0x3FUL)

#define moved_piece(mv)                  ((mv._mv >> 12) & 0xFUL)

#define promotion_piece(mv)              ((mv._mv >> 16) & 0xFUL)

#define capture_flag(mv)                 ((mv._mv >> 20) & 0x1UL)

#define castle_flag(mv)                  ((mv._mv >> 21) & 0x1UL)

#define enpassant_flag(mv)               ((mv._mv >> 22) & 0x1UL)

#define double_pawn_flag(mv)             ((mv._mv >> 23 & 0x1UL))

#define castle_overrides(mv)             ((mv._mv >> 24) & 0xFUL)

#define get_bit(board, square)           (board & (1ULL << square))

#define push_bit(board, square)          (board | (1ULL << square))

#define pop_bit(board, square)           (board & ~(1ULL << square))

/*      print bitboard (debug tool)     */

/// @brief prints a bitboard
/// @param bb
/// @return a string
std::string print_bitboard(const uint64_t& bb) {
  std::string str = "";
  for (int i = 0; i < 64; i++) {
    str += (get_bit(bb, i)) ? '1' : '0';
    if (i % 8 == 7) {
      str += '\n';
    }
  }
  return str;
}

/*      counting and finding bits       */

/// @brief counts the bits in a bitboard
/// @param bitboard
/// @return the number of bits
static inline unsigned int count_bits(const uint64_t& bitboard) {
  uint64_t test = bitboard;
  unsigned int bitnumber = 0;
  while (test) {
    test &= test - 1;
    bitnumber++;
  }
  return bitnumber;
}

/// @brief  finds the index of the least significant bit
/// @param test
/// @return the index
static inline int LSB_index(const uint64_t& test) { return (test) ? count_bits((test & -test) - 1) : -1; }

/*      pack moves      */

/// @brief packs a move_wrapper struct (1hv assumed)
/// @param start_square
/// @param end_square
/// @param moved_piece
/// @param promotion_piece
/// @param capture
/// @param castle
/// @param enpassant
/// @param pp_flag
/// @param castle_ov
/// @return a move_wrapper
static inline move_wrapper pack_move(const unsigned int& start_square, const unsigned int& end_square,
                                     const unsigned int& moved_piece, const unsigned int& promotion_piece,
                                     const unsigned int& capture, const unsigned int& castle,
                                     const unsigned int& enpassant, const unsigned int& pp_flag,
                                     const unsigned int& castle_ov) {
  return {((start_square & 0x3FUL) | ((end_square & 0x3FUL) << 6) | ((moved_piece & 0xFUL) << 12) |
           ((promotion_piece & 0xFUL) << 16) | ((capture & 0x1UL) << 20) | ((castle & 0x1UL) << 21) |
           ((enpassant & 0x1UL) << 22) | ((pp_flag & 0x1UL) << 23) | ((castle_ov & 0xFUL) << 24)),
          1};
}

/*      search constants    */

static constexpr int NUM_THREADS = 1;
static constexpr bool MULTITHREADING_ENABLED =
    false;  // do not enable multithreading it deletes the home directory (not really but it won't work)
static constexpr int MAX_DEPTH = 256;

/*************************************\
|           TIME CONTROLS             |
\*************************************/

/// @brief time managing struct
struct time_controls
{
  int _quit = 0;
  int _movestogo = 30;
  int _movetime = -1;
  int _time = -1;
  int _inc = 0;
  std::chrono::_V2::system_clock::time_point _starttime = high_resolution_clock::now();
  int _stoptime = 0;
  int _timeset = 0;
  int _stopped = 0;
};

/// @brief public time
time_controls pub_time;

/// @brief resets time
static inline void reset_time() {
  pub_time._quit = 0;
  pub_time._movestogo = 30;
  pub_time._movetime = -1;
  pub_time._time = -1;
  pub_time._inc = 0;
  pub_time._starttime = high_resolution_clock::now();
  pub_time._timeset = 0;
  pub_time._stopped = 0;
}

/*************************************\
|           BOARD DEFINITION          |
\*************************************/

/*      board class        */

/* board data is stored as

    enpa sant
    full move
    full move
    half move
    cast ling
    0000 0000
    0000 0000
    0000 000s // s is the side to move

*/

/// @brief Board class with board stuff
class Board
{
 private:
  std::array<uint64_t, 14> board;  // contains bitboards for {P, N, B, R, Q, K, p, n, b, r, q, k, data, hash key}
 public:
  /// @brief constructr for class Board
  inline Board() noexcept : board{0ULL} {}

  /// @brief constructr for class Board
  inline explicit Board(const Board& bd) noexcept : board{std::move(bd.board)} {};

  /// @brief constructr for class Board
  inline explicit Board(Board& bd) noexcept : board{std::move(bd.board)} {};

  /// @brief retrieve a reference to a location on a board
  /// @param index
  /// @return the reference to the bitboard at the index
  /// @note index should be between 0 and 11; 12 and 13 is undefined and anything else may segfault
  inline uint64_t& operator[](const int index) { return board[index]; }

  /// @brief retrieve a location on a board
  /// @param index
  /// @return the value of the bitboard at the index
  /// @note index should be between 0 and 11; 12 and 13 is undefined and anything else may segfault
  inline const uint64_t operator[](const int index) const { return board[index]; }

  inline uint64_t whites() const { return board[0] | board[1] | board[2] | board[3] | board[4] | board[5]; }

  inline uint64_t blacks() const { return board[6] | board[7] | board[8] | board[9] | board[10] | board[11]; }

  inline uint64_t all() const { return this->whites() | this->blacks(); }

  inline uint64_t get_enpassant_square() const  // 255 if none
  {
    return (board[12] & 0xFFULL);
  }

  inline void set_enpassant_square(const uint64_t new_value)  // 255 if none
  {
    board[12] &= ~0xFFULL;
    board[12] |= (new_value & 0xFFULL);
  }

  inline uint64_t get_fullmoves() const { return ((board[12] >> 8) & 0xFFFFULL); }

  inline void set_fullmoves(const uint64_t new_value) {
    board[12] &= ~0xFFFF00ULL;
    board[12] |= (new_value & 0xFFFFULL) << 8;
  }

  inline uint64_t get_halfmoves() const { return (board[12] >> 24) & 0xFFULL; }

  inline void set_halfmoves(const uint64_t new_value) {
    board[12] &= ~0xFF000000ULL;
    board[12] |= (new_value & 0xFFULL) << 24;
  }

  inline uint64_t get_castles() const { return (board[12] >> 32) & 0xFFULL; }

  inline void set_castles(const uint64_t new_value) {
    board[12] &= ~0xFF00000000ULL;
    board[12] |= ((new_value & 0xFFULL) << 32);
  }

  inline void override_castles(const uint64_t override) { board[12] &= (~0ULL) & ~((override & 0xFFULL) >> 32); }

  inline int get_side_to_move() const { return ((board[12] >> 63)) ? 1 : -1; }

  inline void set_side_to_move(const int new_side_to_move) {
    if (get_side_to_move() != new_side_to_move) flip_side();
  }

  inline void flip_side() { board[12] ^= 3ULL << 63; }

  inline void make_move(const move_wrapper& move) {
    set_halfmoves(get_halfmoves() + 1);

    const int end = end_square(move);
    const int start = start_square(move);
    const int moving_side = (get_side_to_move() == 1) ? 0 : 6;

    if (!moving_side) set_fullmoves(get_fullmoves() + 1);

    flip_side();
    override_castles(castle_overrides(move));
    set_enpassant_square(255);

    if (moved_piece(move) == 0 && !moving_side) {
      if (double_pawn_flag(move)) set_enpassant_square(start - 8);
      set_halfmoves(0);
    }

    else if (moved_piece(move) == 0) {
      if (double_pawn_flag(move)) set_enpassant_square(start + 8);
      set_halfmoves(0);
    }

    board[moving_side + moved_piece(move)] ^= (1ULL << start);

    if capture_flag (move) {
      std::for_each(&board[6 - moving_side], &board[12 - moving_side],
                    [=](uint64_t& bb) mutable { bb &= ~(1ULL << end); });
      set_halfmoves(0);
    }

    if (promotion_piece(move)) {
      board[moving_side + promotion_piece(move)] |= 1ULL << end;
    }

    else {
      board[moving_side + moved_piece(move)] |= 1ULL << end;
    }

    if (castle_flag(move)) {
      if (moving_side) {
        board[3] = push_bit(board[3], ((end + start) / 2));
        board[3] = pop_bit(board[3], ((end > start) ? _H1 : _A1));
      }

      else {
        board[9] = push_bit(board[9], ((end + start) / 2));
        board[9] = pop_bit(board[9], ((end > start) ? _H8 : _A8));
      }
    }
    if (enpassant_flag(move)) {
      board[0] &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
      board[6] &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
    }
  }
};

/// @brief loads a position from a FEN code
/// @param bd a pointer to a Board
/// @param FEN
void import_FEN(Board* bd, const std::string& FEN) {
  char arr[64];
  int index = 0;
  int feni = 0;
  for (feni; FEN[feni] != ' '; feni++) {
    if (FEN[feni] == '/') {
      continue;
    } else if (std::isdigit(FEN[feni])) {
      for (int i = 0; i <= (FEN[feni] - '1'); i++) {
        arr[index] = '\0';
        index++;
      }
    } else {
      arr[index] = FEN[feni];
      index++;
    }
  }
  for (int i = 0; i < 12; i++) {
    (*bd)[i] = 0ULL;
  }
  for (int pos = 0; pos < 64; pos++) {
    uint64_t itr = 1ULL << pos;
    switch (arr[pos]) {
      case 'P':
        (*bd)[0] |= itr;
        break;
      case 'N':
        (*bd)[1] |= itr;
        break;
      case 'B':
        (*bd)[2] |= itr;
        break;
      case 'R':
        (*bd)[3] |= itr;
        break;
      case 'Q':
        (*bd)[4] |= itr;
        break;
      case 'K':
        (*bd)[5] |= itr;
        break;

      case 'p':
        (*bd)[6] |= itr;
        break;
      case 'n':
        (*bd)[7] |= itr;
        break;
      case 'b':
        (*bd)[8] |= itr;
        break;
      case 'r':
        (*bd)[9] |= itr;
        break;
      case 'q':
        (*bd)[10] |= itr;
        break;
      case 'k':
        (*bd)[11] |= itr;
        break;
    }
  }
  feni++;
  if (FEN[feni] == 'w') {
    bd->set_side_to_move(1);
  } else {
    bd->set_side_to_move(-1);
  }
  feni += 2;
  uint64_t current_castles = 0ULL;
  for (feni; FEN[feni] != ' '; feni++) {
    switch (FEN[feni]) {
      case 'K':
        current_castles |= 0b00001000ULL;
        break;
      case 'Q':
        current_castles |= 0b00000100ULL;
        break;
      case 'k':
        current_castles |= 0b00000010ULL;
        break;
      case 'q':
        current_castles |= 0b00000001ULL;
        break;
    }
  }
  bd->set_castles(current_castles);
  feni++;
  if (FEN[feni] != '-') {
    bd->set_enpassant_square(0);
    bd->set_enpassant_square(bd->get_enpassant_square() + FEN[feni] - 'a');
    feni++;
    bd->set_enpassant_square(bd->get_enpassant_square() + (8 * (7 - (FEN[feni] - '1'))));
  } else {
    bd->set_enpassant_square(255);
  }
  feni += 2;
  bd->set_fullmoves(0);
  bd->set_halfmoves(0);
  do {
    bd->set_halfmoves(bd->get_halfmoves() * 10ULL);
    bd->set_halfmoves(bd->get_halfmoves() + (uint64_t)(FEN[feni] - '0'));
  } while (isdigit(FEN[++feni]));
  feni++;
  do {
    bd->set_fullmoves(bd->get_fullmoves() * 10ULL);
    bd->set_fullmoves(bd->get_fullmoves() + (uint64_t)(FEN[feni] - '0'));
  } while (isdigit(FEN[++feni]));
}

/// @brief nicely formats a board
/// @param bd a pointer
/// @return a string of the board
std::string print_board(Board* bd) {
  std::string boardstr = "--|---|---|---|---|---|---|--\n";
  const char p[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
  for (int pos = 0; pos < 64; pos++) {
    uint64_t itr = std::pow(2, pos);
    int piece;
    for (piece = 0; piece < 12; piece++) {
      if ((*bd)[piece] & itr) {
        boardstr += p[piece];
        break;
      }
    }
    if (piece != 6) {
      ;
    } else if ((pos + (pos / 8)) % 2 == 1) {
      boardstr += '.';
    } else {
      boardstr += ' ';
    }

    if (pos % 8 == 7)
      boardstr += "\n--|---|---|---|---|---|---|--\n";
    else
      boardstr += " | ";
  }

  return boardstr;
}

/// @brief converts the board to a string
/// @param bd
/// @return a string version of the board
std::string to_string(Board* bd) {
  std::string bdstr = "";
  char p[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
  for (int pos = 0; pos < 64; pos++) {
    uint64_t itr = std::pow(2, pos);
    int piece;
    for (piece = 0; piece < 12; piece++) {
      if ((*bd)[piece] & itr) {
        bdstr += p[piece];
        break;
      }
    }
    if (piece != 12)
      ;
    else if ((pos + (pos / 8)) % 2 == 1) {
      bdstr += '.';
    } else {
      bdstr += ' ';
    }

    if (pos % 8 == 7)
      bdstr += "\n";
    else
      bdstr += " ";
  }
  bdstr += "side to move: ";
  if (bd->get_side_to_move() == 1)
    bdstr += "white";
  else
    bdstr += "black";
  bdstr += "\ncastling ability: ";
  if (bd->get_castles() & 0b00001000) bdstr += "K";
  if (bd->get_castles() & 0b00000100) bdstr += "Q";
  if (bd->get_castles() & 0b00000010) bdstr += "k";
  if (bd->get_castles() & 0b00000001) bdstr += "q";
  bdstr += "\nenpassant ability: " + std::to_string(bd->get_enpassant_square());
  bdstr += "\nmove #" + std::to_string(bd->get_fullmoves());
  bdstr += "\nhalfmoves: " + std::to_string(bd->get_halfmoves());
  bdstr += "\n";

  return bdstr;
}

/*************************************\
|           MAKE/UNMAKE BOARD         |
\*************************************/

class BoardManager
{
 private:
  std::array<Board, MAX_DEPTH> history;
  Board* top;

 public:
  BoardManager(const Board& bd) {
    history = std::array<Board, MAX_DEPTH>();
    history.fill(Board());
    history[0] = std::move(bd);
    top = &history[0];
  }

  inline void make_move(const move_wrapper& mw) {
    *(top + 1) = *top;
    top++;
    top->make_move(mw);
  }

  inline void unmake_move() { top--; }

  inline Board& get_top() { return (*top); }

  inline void null_move() {
    *(top + 1) = *top;
    top++;
    top->flip_side();
  }
};

/*************************************\
|       MOVE & SEARCH CONSTANTS       |
\*************************************/

namespace chess_algo
{
/// @brief length of best and worst moves
static constexpr int FAIL_CUT = 1;

static constexpr int NULL_R = 2;
}  // namespace chess_algo

/*************************************\
|           MOVE GENERATION           |
\*************************************/

/*      attack masks        */

/// @brief find an attack mask for a pawn
/// @param square
/// @param side 1 for white, -1 for black
/// @return a bitboard mask
uint64_t pawn_mask(int square, int side)  // side is 1 for white, -1 for black
{
  uint64_t att = 0ULL;       // attacks
  uint64_t bitboard = 0ULL;  // pawn square
  bitboard = push_bit(bitboard, square);
  if (side == 1)  // white's attacks
  {
    att |= ((bitboard & A_FILE) ? 0ULL : (bitboard >> 9)) | ((bitboard & H_FILE) ? 0ULL : (bitboard >> 7));
  } else  // black's attacks
  {
    att |= ((bitboard & A_FILE) ? 0ULL : (bitboard << 7)) | ((bitboard & H_FILE) ? 0ULL : (bitboard << 9));
  }
  return att;
}

/// @brief find a mask for a knight
/// @param square
/// @return a bitboard mask
uint64_t knight_mask(int square) {
  uint64_t att = 0ULL;       // attacks
  uint64_t bitboard = 0ULL;  // knight square
  bitboard = push_bit(bitboard, square);
  // generate 17 and -15 attacks
  if (!(bitboard & A_FILE)) {
    att |= (bitboard >> 17) | (bitboard << 15);
  }
  // generate -17 and 15 attacks
  if (!(bitboard & H_FILE)) {
    att |= (bitboard << 17) | (bitboard >> 15);
  }
  // generate 10 and -6 attacks
  if (!((bitboard & A_FILE) || (bitboard & B_FILE))) {
    att |= (bitboard >> 10) | (bitboard << 6);
  }
  // generate -10 and 6 attacks
  if (!((bitboard & H_FILE) || (bitboard & G_FILE))) {
    att |= (bitboard << 10) | (bitboard >> 6);
  }
  return att;
}

/// @brief find a mask for a king
/// @param square
/// @return a bitboard mask
uint64_t king_mask(int square) {
  uint64_t att = 0ULL;       // attacks
  uint64_t bitboard = 0ULL;  // king square
  bitboard = push_bit(bitboard, square);
  if (!(bitboard & H_FILE)) {
    att |= (bitboard << 9) | (bitboard << 1) | (bitboard >> 7);
  }
  if (!(bitboard & A_FILE)) {
    att |= (bitboard >> 9) | (bitboard >> 1) | (bitboard << 7);
  }
  att |= (bitboard << 8) | (bitboard >> 8);
  return att;
}

/// @brief find a mask for bishop occupancies
/// @param square
/// @return a bitboard mask
uint64_t bishop_mask(int square) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  uint64_t att = 0ULL;
  for (rank = t_rank + 1, file = t_file + 1; rank <= 6 && file <= 6; rank++, file++) att |= 1ULL << (rank * 8 + file);
  for (rank = t_rank - 1, file = t_file + 1; rank >= 1 && file <= 6; rank--, file++) att |= 1ULL << (rank * 8 + file);
  for (rank = t_rank + 1, file = t_file - 1; rank <= 6 && file >= 1; rank++, file--) att |= 1ULL << (rank * 8 + file);
  for (rank = t_rank - 1, file = t_file - 1; rank >= 1 && file >= 1; rank--, file--) att |= 1ULL << (rank * 8 + file);
  return att;
}

/// @brief find a mask for rook occupancies
/// @param square
/// @return a bitboard mask
uint64_t rook_mask(int square) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  uint64_t att = 0ULL;
  for (rank = t_rank + 1; rank <= 6; rank++) att |= (1ULL << (rank * 8 + t_file));
  for (rank = t_rank - 1; rank >= 1; rank--) att |= (1ULL << (rank * 8 + t_file));
  for (file = t_file + 1; file <= 6; file++) att |= (1ULL << (t_rank * 8 + file));
  for (file = t_file - 1; file >= 1; file--) att |= (1ULL << (t_rank * 8 + file));
  return att;
}

/// @brief silent masks for pawns
/// @param square
/// @param side
/// @return a bitboard mask
uint64_t silent_pawn_mask(int square, int side) {
  uint64_t mv = 0ULL;
  uint64_t bitboard = 0ULL;  // pawn square
  bitboard = push_bit(bitboard, square);
  if (side == 1) {
    mv |= (bitboard >> 8);
    if (square / 8 == 6) mv |= (bitboard >> 16);
  } else {
    mv |= (bitboard << 8);
    if (square / 8 == 1) mv |= (bitboard << 16);
  }
  return mv;
}

/*      on the fly attacks and other magic generation tools     */

/// @brief on the fly attacks for bishops
/// @param square
/// @param blockers
/// @return an attack bitboard
/// @note for magic generation
uint64_t otf_bishop_attacks(int square, uint64_t blockers) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  uint64_t att = 0ULL;
  for (rank = t_rank + 1, file = t_file + 1; rank <= 7 && file <= 7; rank++, file++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank - 1, file = t_file + 1; rank >= 0 && file <= 7; rank--, file++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank + 1, file = t_file - 1; rank <= 7 && file >= 0; rank++, file--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank - 1, file = t_file - 1; rank >= 0 && file >= 0; rank--, file--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  return att;
}

/// @brief on the fly attacks for rooks
/// @param square
/// @param blockers
/// @return an attack bitboard
/// @note for magic generation
uint64_t otf_rook_attacks(int square, uint64_t blockers) {
  int rank, file;
  int t_rank = square / 8;
  int t_file = square % 8;
  uint64_t att = 0ULL;
  for (rank = t_rank + 1, file = t_file; rank <= 7; rank++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank - 1, file = t_file; rank >= 0; rank--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank, file = t_file + 1; file <= 7; file++) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  for (rank = t_rank, file = t_file - 1; file >= 0; file--) {
    att |= 1ULL << (rank * 8 + file);
    if ((1ULL << (rank * 8 + file)) & blockers) break;
  }
  return att;
}

/// @brief gets the occupancy for a mask
/// @param index
/// @param bit_count
/// @param mask
/// @return a bitboard
/// @note for magic generation
uint64_t get_occupancy(int index, int bit_count, uint64_t mask) {
  uint64_t occupancy = 0ULL;

  for (int i = 0; i < bit_count; i++) {
    const int square = LSB_index(mask);
    mask = pop_bit(mask, square);
    if (index & (1ULL << i)) {
      occupancy |= (1ULL << square);
    }
  }

  return occupancy;
}

/// @brief finds a magic number
/// @param square
/// @param occupancy
/// @param piece
/// @return a magic number
uint64_t find_magic(int square, int occupancy, int piece) {
  uint64_t occupancies[4096];
  uint64_t attacks[4096];
  uint64_t checked_attacks[4096];

  uint64_t attack_mask = piece ? bishop_mask(square) : rook_mask(square);

  int occupancy_index = 1 << occupancy;

  for (int i = 0; i < occupancy_index; i++) {
    occupancies[i] = get_occupancy(i, occupancy, attack_mask);

    attacks[i] = piece ? otf_bishop_attacks(square, occupancies[i]) : otf_rook_attacks(square, occupancies[i]);
  }

  for (int i = 0; i < 10000000; i++) {
    uint64_t candidate = magic_candidate();
    if (count_bits((attack_mask * candidate) & 0xFF00000000000000) < 6) continue;
    memset(checked_attacks, 0ULL, sizeof(checked_attacks));

    int index, fail;
    for (index = 0, fail = 0; !fail && index < occupancy_index; index++) {
      int magic_index = (occupancies[index] * candidate) >> (64 - occupancy);

      if (checked_attacks[magic_index] == 0ULL)
        checked_attacks[magic_index] = attacks[index];

      else if (checked_attacks[magic_index] != attacks[index])
        fail = 1;
    }

    if (!fail) return candidate;
  }
  return 0;
}

/*      initialization routines         */

/// @brief initalizes magic numbers
/// @note does not do anything, magics are already generated and known at compile-time
void initialize_magic_numbers() {
  for (int i = 0; i < 64; i++) {
    // rook_magics[i] = find_magic(i, rook_occupancy_bits[i], mROOK);
  }
  for (int i = 0; i < 64; i++) {
    // bishop_magics[i] = find_magic(i, bishop_occupancy_bits[i], mBISHOP);
  }
}

/// @brief initialize attack tables for non-slider pieces (pawns, knights, kings)
void initialize_non_sliders() {
  for (int i = 0; i < 64; i++) {
    pawn_attacks[0][i] = pawn_mask(i, -1);
    pawn_attacks[1][i] = pawn_mask(i, 1);
    knight_attacks[i] = knight_mask(i);
    king_attacks[i] = king_mask(i);
    silent_pawn_moves[0][i] = silent_pawn_mask(i, -1);
    silent_pawn_moves[1][i] = silent_pawn_mask(i, 1);
  }
}

/// @brief initialize attack tables for slider pieces (bishops, rooks, queens)
void initialize_sliders() {
  for (int i = 0; i < 64; i++) {
    bishop_attack_masks[i] = bishop_mask(i);
    rook_attack_masks[i] = rook_mask(i);

    uint64_t b_attack_mask = bishop_attack_masks[i];
    uint64_t r_attack_mask = rook_attack_masks[i];

    int b_bits = count_bits(b_attack_mask);
    int r_bits = count_bits(r_attack_mask);

    int b_occi = 1 << b_bits;
    int r_occi = 1 << r_bits;

    for (int index = 0; index < b_occi; index++) {
      uint64_t occ = get_occupancy(index, b_bits, b_attack_mask);

      int magic_index = (occ * bishop_magics[i]) >> (64 - bishop_occupancy_bits[i]);

      magic_bishop_attacks[i][magic_index] = otf_bishop_attacks(i, occ);
    }

    for (int index = 0; index < r_occi; index++) {
      uint64_t occ = get_occupancy(index, r_bits, r_attack_mask);

      int magic_index = (occ * rook_magics[i]) >> (64 - rook_occupancy_bits[i]);

      magic_rook_attacks[i][magic_index] = otf_rook_attacks(i, occ);
    }
  }
}

/*      slider attack lookups       */

/// @brief gets bishop attacks for an occupancy
/// @param square
/// @param occupancy
/// @return an attack bitboard
static inline uint64_t get_bishop_attacks(const int square, uint64_t occupancy) {
  occupancy &= bishop_attack_masks[square];
  occupancy *= bishop_magics[square];
  occupancy >>= 64 - bishop_occupancy_bits[square];
  return magic_bishop_attacks[square][occupancy];
}

/// @brief gets rook attacks for an occupancy
/// @param square
/// @param occupancy
/// @return an attack bitboard
static inline uint64_t get_rook_attacks(const int square, uint64_t occupancy) {
  occupancy &= rook_attack_masks[square];
  occupancy *= rook_magics[square];
  occupancy >>= 64 - rook_occupancy_bits[square];
  return magic_rook_attacks[square][occupancy];
}

/// @brief gets queen attacks for an occupancy
/// @param square
/// @param occupancy
/// @return an attack bitboard
static inline uint64_t get_queen_attacks(const int square, const uint64_t& occupancy) {
  uint64_t occ1 = occupancy;

  occ1 &= bishop_attack_masks[square];
  occ1 *= bishop_magics[square];
  occ1 >>= 64 - bishop_occupancy_bits[square];

  uint64_t occ2 = occupancy;

  occ2 &= rook_attack_masks[square];
  occ2 *= rook_magics[square];
  occ2 >>= 64 - rook_occupancy_bits[square];

  return magic_rook_attacks[square][occ2] | magic_bishop_attacks[square][occ1];
}

/*      check and attack routines       */

/// @brief checks if a square is attacked
/// @param square
/// @param bd
/// @param attacking_side
/// @return whether the square is attacked or not
static inline const bool is_attacked(int square, const Board& bd, const int attacking_side) {
  return (square == -1)
             ? false
             : ((attacking_side == 1)
                    ? ((pawn_attacks[0][square] & bd[0]) || (knight_attacks[square] & bd[1]) ||
                       (get_bishop_attacks(square, bd.all()) & (bd[2] | bd[4])) ||
                       (get_rook_attacks(square, bd.all()) & (bd[3] | bd[4])) || (king_attacks[square] & bd[5]))
                    : ((pawn_attacks[1][square] & bd[6]) || (knight_attacks[square] & bd[7]) ||
                       (get_bishop_attacks(square, bd.all()) & (bd[8] | bd[10])) ||
                       (get_rook_attacks(square, bd.all()) & (bd[9] | bd[10])) || (king_attacks[square] & bd[11])));
}

/// @brief scores a move for move ordering
/// @param bd
/// @param move
/// @param tst
/// @return the heuristic score (currently unused)
static inline int heuristic_score(const Board& bd, const unsigned int move, Board& tst) {
  // does nothing
  return 0;
}

/// @brief checks if a side is in check
/// @param bd
/// @param side_in_check
/// @return whether the side is in check
static inline const bool in_check(const Board& bd, const int side_in_check) {
  return ((side_in_check == 1) ? is_attacked(LSB_index(bd[5]), bd, -1) : is_attacked(LSB_index(bd[11]), bd, 1));
}

/// @brief checks whether a move would put the moving side in check
/// @param bd
/// @param move
/// @param tst
/// @return boolean value
static inline const bool check_check(const Board& bd, const unsigned int move, Board& tst) {
  tst = bd;
  ;
  tst.make_move({move, 0});
  return in_check(tst, -tst.get_side_to_move());
}

/*      move generation routine     */

/// @brief generate all legal moves
/// @param bd
/// @param start_pointer
/// @return the size of the array
static inline unsigned int get_moves(const Board& bd, move_wrapper* start_pointer) {
  // clear array
  unsigned int size = 0;

  std::fill(start_pointer, start_pointer + ARRAY_SIZE, move_wrapper());

  // initiallize constants
  const uint64_t all_pieces = bd.all();
  const int side = bd.get_side_to_move();
  const int binary_side = (side == 1) ? 1 : 0;
  const uint64_t attacking_occ = (side == 1) ? bd.whites() : bd.blacks();
  const uint64_t defending_occ = (side == 1) ? bd.blacks() : bd.whites();

  // set pointers for attacking and defending pieces
  const int attacking_pieces = (side == -1) * 6;
  const int defending_pieces = (side == 1) * 6;

  // generate pawn moves
  uint64_t piece_board = bd[PAWN + attacking_pieces];
  uint64_t att = 0ULL;
  while (piece_board) {
    const int square = LSB_index(piece_board);
    piece_board = pop_bit(piece_board, square);
    att |= (pawn_attacks[binary_side][square] & (defending_occ | (1ULL << bd.get_enpassant_square()))) |
           (silent_pawn_moves[binary_side][square] & ~all_pieces);
    att &= ~attacking_occ;
    while (att) {
      const int j = LSB_index(att);
      att = pop_bit(att, j);
      const int pp_flag = (abs(square - j) == 16) ? 0b1 : 0b0;
      uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
      uint16_t enpassantflg = (j == bd.get_enpassant_square()) ? 0b1 : 0;
      move_wrapper move = pack_move(square, j, (uint32_t)PAWN, 0U, captureflg, 0U, enpassantflg, pp_flag, 0b1111U);

      // calculate heristics
      uint16_t heuristic_value = 10U;

      // check for promotions
      if ((j / 8 == 0) || (j / 8 == 7)) {
        for (uint32_t promotee = 1UL; promotee < 5UL; promotee++) {
          move = pack_move(square, j, (uint32_t)PAWN, promotee, captureflg, 0U, enpassantflg, pp_flag, 0b1111U);
          set_heuristic(move, (heuristic_value + (captureflg * 2) + (promotee * 4)));
          arradd(start_pointer, size, move);
        }
      } else {
        set_heuristic(move, (heuristic_value + (enpassantflg) + (captureflg * 5)));
        arradd(start_pointer, size, move);
      }
    }
  }

  // generate knight moves
  piece_board = bd[KNIGHT + attacking_pieces];
  att = 0ULL;
  while (piece_board) {
    const int square = LSB_index(piece_board);
    piece_board = pop_bit(piece_board, square);
    att |= knight_attacks[square];
    att &= ~attacking_occ;
    while (att) {
      const int j = LSB_index(att);
      att = pop_bit(att, j);
      uint32_t captureflg = ((1ULL << j) & defending_occ) ? 1U : 0U;
      move_wrapper move = pack_move(square, j, (uint32_t)KNIGHT, 0UL, captureflg, 0UL, 0UL, 0UL, 0b1111UL);

      // calculate heristics
      uint16_t heuristic_value = 10U;

      set_heuristic(move, (heuristic_value + (captureflg * 5)));
      arradd(start_pointer, size, move);
    }
  }

  // generate bishop moves
  piece_board = bd[BISHOP + attacking_pieces];
  att = 0ULL;
  while (piece_board) {
    const int square = LSB_index(piece_board);
    piece_board = pop_bit(piece_board, square);
    att |= get_bishop_attacks(square, all_pieces);
    att &= ~attacking_occ;
    while (att) {
      const int j = LSB_index(att);
      att = pop_bit(att, j);
      uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
      move_wrapper move = pack_move(square, j, (uint32_t)BISHOP, 0UL, captureflg, 0UL, 0UL, 0UL, 0b1111UL);

      // calculate heristics
      uint16_t heuristic_value = 10U;

      set_heuristic(move, (heuristic_value + (captureflg * 5)));
      arradd(start_pointer, size, move);
    }
  }

  // generate rook moves
  piece_board = bd[ROOK + attacking_pieces];
  att = 0ULL;
  while (piece_board) {
    const int square = LSB_index(piece_board);
    piece_board = pop_bit(piece_board, square);
    att |= get_rook_attacks(square, all_pieces);
    att &= ~attacking_occ;

    // get castling rights
    int castle = 0b11;
    if ((square % 8) == 0)
      castle &= 0b10;
    else if ((square % 8) == 0)
      castle &= 0b01;
    uint32_t castles_ov = (binary_side) ? (0b11 | (castle << 2)) : (0b1100 | castle);

    while (att) {
      const int j = LSB_index(att);
      att = pop_bit(att, j);
      uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
      move_wrapper move = pack_move(square, j, (uint32_t)ROOK, 0UL, captureflg, 0UL, 0UL, 0UL, castles_ov);

      // calculate heristics
      uint16_t heuristic_value = 10U;

      set_heuristic(move, (heuristic_value + (captureflg * 5)));
      arradd(start_pointer, size, move);
    }
  }

  // generate queen moves
  piece_board = bd[QUEEN + attacking_pieces];
  att = 0ULL;
  while (piece_board) {
    const int square = LSB_index(piece_board);
    piece_board = pop_bit(piece_board, square);
    att |= get_queen_attacks(square, all_pieces);
    att &= ~attacking_occ;
    while (att) {
      const int j = LSB_index(att);
      att = pop_bit(att, j);
      uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
      move_wrapper move = pack_move(square, j, (uint32_t)QUEEN, 0UL, captureflg, 0UL, 0UL, 0UL, 0b1111UL);

      // calculate heristics
      uint16_t heuristic_value = 10ULL;

      set_heuristic(move, (heuristic_value + (captureflg * 5)));
      arradd(start_pointer, size, move);
    }
  }

  // generate king moves
  piece_board = bd[KING + attacking_pieces];
  att = 0ULL;
  while (piece_board) {
    const int square = LSB_index(piece_board);
    piece_board = pop_bit(piece_board, square);
    att |= king_attacks[square];
    att &= ~attacking_occ;

    // get castling rights
    uint32_t castles_ov = (binary_side) ? (0b0011) : (0b1100);

    while (att) {
      const int j = LSB_index(att);
      att = pop_bit(att, j);
      uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
      move_wrapper move = pack_move(square, j, (uint32_t)KING, 0UL, captureflg, 0UL, 0UL, 0UL, castles_ov);

      // verify checks

      // calculate heristics
      uint64_t heuristic_value = 10ULL;

      set_heuristic(move, (heuristic_value + (captureflg * 5)));
      arradd(start_pointer, size, move);
    }
  }

  // castling routine
  if (side == 1) {
    if (bd.get_castles() & 0b1000) {
      if (!(is_attacked(_E1, bd, -1) || is_attacked(_F1, bd, -1) || is_attacked(_G1, bd, -1)) &&
          !(all_pieces & (_F1 | _G1))) {
        arradd(start_pointer, size, pack_move(_E1, _G1, KING, 0U, 0U, 1U, 0U, 0U, 0b0011));
      }
    }
    if (bd.get_castles() & 0b0100) {
      if (!(is_attacked(_E1, bd, -1) || is_attacked(_D1, bd, -1) || is_attacked(_C1, bd, -1)) &&
          !(all_pieces & (_D1 | _C1 | _B1))) {
        arradd(start_pointer, size, pack_move(_E1, _C1, KING, 0U, 0U, 1U, 0U, 0U, 0b0011));
      }
    }
  } else {
    if (bd.get_castles() & 0b0010) {
      if (!(is_attacked(_E8, bd, 1) || is_attacked(_F8, bd, 1) || is_attacked(_G8, bd, 1)) &&
          !(all_pieces & (_F8 | _G8))) {
        arradd(start_pointer, size, pack_move(_E8, _G8, KING, 0U, 0U, 1U, 0U, 0U, 0b1100));
      }
    }
    if (bd.get_castles() & 0b0001) {
      if (!(is_attacked(_E8, bd, 1) || is_attacked(_D8, bd, 1) || is_attacked(_C1, bd, 1)) &&
          !(all_pieces & (_D8 | _C8 | _B8))) {
        arradd(start_pointer, size, pack_move(_E8, _C8, KING, 0U, 0U, 1U, 0U, 0U, 0b1100));
      }
    }
  }

  // exit if empty
  if (size == 0) return 0;

  // remove illegal moves
  Board tst;
  std::for_each(start_pointer, start_pointer + size, [&](move_wrapper& mv) mutable {
    tst = Board(bd);
    tst.make_move(mv);
    if (in_check(tst, -bd.get_side_to_move())) mv = {0, 0};
  });

  // sort and finalize heuristics
  std::sort(start_pointer, start_pointer + ARRAY_SIZE, std::greater<>());
  const int final_size =
      std::find_if(start_pointer, start_pointer + ARRAY_SIZE, [](move_wrapper mv) -> bool { return (mv._mv != 0); }) -
      start_pointer;
  const auto num_best = std::min(final_size / chess_algo::FAIL_CUT, chess_algo::FAIL_CUT);
  const auto num_killer = std::min(final_size - num_best, chess_algo::FAIL_CUT);

  // set best and killer values
  std::for_each(start_pointer, start_pointer + num_best, [&](move_wrapper& mv) mutable { mv._hv |= (1UL << 15); });
  std::for_each(start_pointer + (final_size - num_killer), start_pointer + final_size,
                [&](move_wrapper& mv) mutable { mv._hv |= (1UL << 14); });
  std::swap_ranges(start_pointer + (final_size - num_killer), start_pointer + final_size, start_pointer + num_best);

  // return size of the array
  return final_size;
}

/*      move checking routine       */

/// @brief prints the move and heuristic values for a board
/// @param bd
void print_move_values(const Board& bd) {
  int i = 1;
  printf("\n\t");
  std::array<move_wrapper, 120> arr = std::array<move_wrapper, 120>();
  get_moves(bd, arr.begin());
  for (move_wrapper mw : arr) {
    unsigned int j = mw._mv;
    printf("%i:%x\t->%i\n\t", i++, j, mw._hv);
  }
  printf("\n");
}

/*************************************\
|           SEARCH & PERFT            |
\*************************************/

static constexpr int infinity = 1000000;
static constexpr int checkmate = 500000;

std::mutex array_lock;

static inline int eval(const Board& bd);

static inline void print_move(const move_wrapper& move, const bool& new_line);

struct move_info
{
  move_wrapper move;
  long long int total_nodes;
  int eval;
  inline bool operator>(const move_info& other) const { return this->eval > other.eval; }
};

void print_move_info(const move_info& mvif) {
  printf("move_info: ");
  print_move(mvif.move, false);
  printf("\tafter %10lli nodes -> %+4.1f\n", mvif.total_nodes, (float)mvif.eval / 100.0);
}

static inline int quescence_search(int alpha, int beta) { return 0; }

struct get_values
{
 private:
  BoardManager _bm{Board()};
  std::array<move_wrapper, ARRAY_SIZE> _move_arrays[MAX_DEPTH];
  int* _ply{nullptr};
  long long int* _total_nodes{nullptr};
  move_wrapper _mvwr;
  int val{0};

  inline int negamax(const int depth, int alpha, int beta, const int side) {
    (*_total_nodes)++;
    // return if done or draw
    if (depth <= 0) {
      return -side * eval(_bm.get_top());
    }
    if (_bm.get_top().get_halfmoves() >= 50) return 0;

    // null move pruning
    _bm.null_move();
    const int prune = -negamax(depth - 1 - chess_algo::NULL_R, -beta, -alpha, -side);
    _bm.unmake_move();
    if (prune >= beta) return beta;

    // create and check moves
    get_moves(_bm.get_top(), _move_arrays[*_ply].begin());
    if (_move_arrays[*_ply][0]._mv == 0) {
      if (in_check(_bm.get_top(), -side))
        return (checkmate + depth);
      else
        return 0;
    }

    /*   start of NegaMax   */
    // set initial value
    (*_ply)++;
    int value = -infinity;
    std::find_if(_move_arrays[*_ply - 1].begin(), _move_arrays[*_ply - 1].end(), [&](move_wrapper i) mutable -> bool {
      // check for null results
      if (i._mv == 0) return true;

      // move the check board
      _bm.make_move(i);

      // recursively evaluate next node
      const int ng = -negamax(depth - 1, -beta, -alpha, -side);

      // unmake move
      _bm.unmake_move();

      // set value as the most valuable node
      value = std::max(ng, value);

      // alpha-beta cutoff
      if (value >= beta) {
        return true;
      }

      // set alpha
      alpha = std::max(alpha, value);
      return false;
    });
    // return the value of the node
    (*_ply)--;
    return value;
  }

 public:
  inline get_values(const move_wrapper& move, const Board& bd) : _mvwr{move} {
    std::for_each(_move_arrays, _move_arrays + MAX_DEPTH,
                  [&](std::array<move_wrapper, ARRAY_SIZE>& arr) { arr = std::array<move_wrapper, ARRAY_SIZE>(); });
    _bm = BoardManager(bd);
    _bm.make_move(move);
    _total_nodes = new long long{0};
    _ply = new int{0};
  }

  inline move_info operator()(const int& depth) {
    val = negamax(depth, -infinity, infinity, _bm.get_top().get_side_to_move());
    return {_mvwr, *_total_nodes, val};
  }

  inline ~get_values() { delete this->_total_nodes, this->_ply; }
};

static inline void run_threads(const int depth, const move_wrapper& mv, const Board& bd,
                               std::vector<move_info>& eval_arr) {
  get_values v(mv, bd);
  move_info mvnfo = v(depth);
  array_lock.lock();
  eval_arr.push_back(mvnfo);
  array_lock.unlock();
}

static inline move_info get_best_move(const Board* bd, int depth) {
  if (bd->get_halfmoves() >= 50) return {{0, 0}, 0LL, 0};

  std::array<move_wrapper, ARRAY_SIZE> arr = std::array<move_wrapper, ARRAY_SIZE>();
  std::vector<move_info> eval_arrays = std::vector<move_info>();
  std::array<std::thread*, NUM_THREADS> threads = std::array<std::thread*, NUM_THREADS>();

  long long int _total_nodes = 0;
  const int size = ARRAY_SIZE;

  get_moves(*bd, arr.begin());

  if (arr[0]._mv == 0) return {{0, 0}, 0, 0};

  const int last_move = ([&]() -> int {
    if (MULTITHREADING_ENABLED) {
      int pos = 0;
      while (arr[pos]._mv) {
        std::for_each(threads.begin(), threads.end(), [&](std::thread* thread) mutable {
          if (!arr[pos]._mv) return; /*thread = new std::thread(run_threads, depth, arr[pos]._mv, *bd, eval_arrays)*/
          ;
          pos++;
        });
        std::for_each(threads.begin(), threads.end(), [&](std::thread* thread) mutable {
          thread->join();
          delete thread;
        });
      }
      return pos;
    } else {
      const auto pos = std::find_if(arr.begin(), arr.end(), [&](const move_wrapper& mv) -> bool {
        if (!mv._mv) return true;
        get_values v(mv, *bd);
        eval_arrays.push_back(v(depth));
        return false;
      });

      return pos - &arr[0];
    }
  })();

  std::for_each(eval_arrays.begin(), eval_arrays.begin() + last_move,
                [&](move_info mi) { _total_nodes += mi.total_nodes; });
  std::sort(eval_arrays.begin(), eval_arrays.begin() + last_move, std::greater<>());

  eval_arrays[0].total_nodes = _total_nodes;
  return {eval_arrays[0]};
}

static inline int get_perft(int depth, int& ply, Board** history, std::array<move_wrapper, ARRAY_SIZE>* move_arrays) {
  if (depth == 0) return 1;
  int sum = 0;
  const int sz = get_moves(*history[ply], move_arrays[ply].begin());
  // print_moves(*history[ply]);
  ply++;
  std::find_if(&move_arrays[ply - 1][0], (&move_arrays[ply - 1][0]) + ARRAY_SIZE, [&](move_wrapper mw) mutable -> bool {
    if (!mw._mv) return true;
    *history[ply] = *history[ply - 1];
    history[ply]->make_move(mw);
    const int inc = get_perft(depth - 1, ply, history, move_arrays);
    if (ply == 1) {
      print_move(mw, false);
      printf("\t%i\n", inc);
    }
    sum += inc;
    return false;
  });
  ply--;
  return sum;
}

void run_perft(const Board& bd) {
  Board* history[MAX_DEPTH];
  std::for_each(history, history + MAX_DEPTH, [&](Board*& bd) mutable { bd = new Board(); });
  std::array<move_wrapper, ARRAY_SIZE> move_arrays[MAX_DEPTH];
  std::for_each(move_arrays, move_arrays + MAX_DEPTH,
                [&](std::array<move_wrapper, ARRAY_SIZE>& arr) { arr = std::array<move_wrapper, ARRAY_SIZE>(); });
  long long int total_nodes = 0;

  for (int i = 0; i < 7; i++) {
    int ply = 0;
    *history[0] = bd;
    const auto begin = high_resolution_clock::now();
    const int perftval = get_perft(i, ply, history, move_arrays);
    const auto end = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(end - begin);
    const float speed = ((float)perftval) / duration.count();

    printf("\n%i:\t%i nodes @%4.1fk nodes/second\n\n", i, perftval, speed);
  }
}

}  // namespace BCdepricated
#endif
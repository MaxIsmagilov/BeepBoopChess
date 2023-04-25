#ifndef BitTools
#define BitTools

#include <bits/stdc++.h>
#include <string>
#include <sstream>

const uint64_t A_FILE = 72340172838076673ULL;

const uint64_t B_FILE = 144680345676153346ULL;

const uint64_t G_FILE = 4629771061636907072ULL;

const uint64_t H_FILE = 9259542123273814144ULL;

enum GAMESTATE {RUNNING, BLACK_CHECKMATE, STALEMATE, WHITE_CHECKMATE};

enum piece_numbers {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

enum colors {WHITE, BLACK};


const unsigned int ARRAY_SIZE = 120;

enum square {
    _A8, _B8, _C8, _D8, _E8, _F8, _G8, _H8, 
    _A7, _B7, _C7, _D7, _E7, _F7, _G7, _H7,
    _A6, _B6, _C6, _D6, _E6, _F6, _G6, _H6,
    _A5, _B5, _C5, _D5, _E5, _F5, _G5, _H5,
    _A4, _B4, _C4, _D4, _E4, _F4, _G4, _H4,
    _A3, _B3, _C3, _D3, _E3, _F3, _G3, _H3,
    _A2, _B2, _C2, _D2, _E2, _F2, _G2, _H2,
    _A1, _B1, _C1, _D1, _E1, _F1, _G1, _H1,
};

struct move_wrapper
{
    uint32_t _mv;
    uint16_t _hv;
    inline bool operator<(const move_wrapper& other) const
    { 
        return this->_hv < other._hv;
    }
    inline bool operator>(const move_wrapper& other) const
    {
        return this->_hv > other._hv;
    }
};


// move encoding guide (using uint32_t wrapper struct)
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

// returns a move_wrapper struct with 1 hv
#define pack_move(start_square, end_square, moved_piece, promotion_piece, capture, castle, enpassant, pp_flag , castle_ov) {((start_square & 0x3FUL)|((end_square & 0x3FUL) << 6) | ((moved_piece&0xFUL) << 12) | ((promotion_piece & 0xFUL) << 16) | ((capture & 0x1UL) << 20) | ((castle & 0x1UL) << 21)| ((enpassant & 0x1UL) << 22)|((pp_flag & 0x1UL) << 23)| ((castle_ov & 0xFUL) << 24)), 1}

#define set_heuristic(mv, new_heuristic) (mv._hv = new_heuristic)

#define start_square(mv) ((mv._mv) & 0x3FUL)

#define end_square(mv) ((mv._mv >> 6) & 0x3FUL)

#define moved_piece(mv) ((mv._mv >> 12) & 0xFUL)

#define promotion_piece(mv) ((mv._mv >> 16) & 0xFUL)

#define capture_flag(mv) ((mv._mv >> 20) & 0x1UL)

#define castle_flag(mv) ((mv._mv >> 21) & 0x1UL)

#define enpassant_flag(mv) ((mv._mv >> 22) & 0x1UL)

#define double_pawn_flag(mv) ((mv._mv >> 23 & 0x1UL))

#define castle_overrides(mv) ((mv._mv >> 24) & 0xFUL)

#define get_bit(board, square) (board & (1ULL << square))

#define push_bit(board, square) (board | (1ULL << square))

#define pop_bit(board, square) (board & ~(1ULL << square))

std::string print_bitboard(uint64_t bb)
{
    std::string str = "";
    for (int i = 0; i < 64; i++)
    {
        str += (get_bit(bb,i)) ? '1' : '0';
        if (i%8 == 7) {str += '\n';}
    }
    return str;
}

static inline unsigned int count_bits(const uint64_t& bitboard)
{
    uint64_t test = bitboard;
    unsigned int bitnumber = 0;
    while (test)
    {
        test &= test - 1;
        bitnumber++;
    }
    return bitnumber;
}

static inline int LSB_index(const uint64_t& test)
{
    return (test) ? count_bits((test &  -test) - 1) : -1;
}

#endif
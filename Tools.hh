#ifndef BitTools
#define BitTools
#include <bits/stdc++.h>
#include <string>
#include <sstream>

const uint64_t A_FILE = 72340172838076673ULL;

const uint64_t B_FILE = 144680345676153346ULL;

const uint64_t G_FILE = 4629771061636907072ULL;

const uint64_t H_FILE = 9259542123273814144ULL;

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

// move encoding guide (using uint32_t)
    // 0000 0000 0000 0000 0000 0000 0111 1111  start square 
    // 0000 0000 0000 0000 0011 1111 1000 0000  end square
    // 0000 0000 0000 0011 1100 0000 0000 0000  promotion piece (0 for none)
    // 0000 0000 0000 0100 0000 0000 0000 0000  capture flag
    // 0000 0000 0000 1000 0000 0000 0000 0000  castling flag
    // 0000 0000 0001 0000 0000 0000 0000 0000  enpassant flag
    // 0000 0001 1110 0000 0000 0000 0000 0000  castle overrides (1111 by default, 0 where no longer available)
    // 1111 1110 0000 0000 0000 0000 0000 0000  ordering priority (1000 000 = best, 0100 000 = killer, rest are other info)

static inline uint32_t pack_move(uint32_t start_square, uint32_t end_square, uint32_t promotion_piece, uint32_t capture, uint32_t castle, uint32_t enpassant, uint32_t castle_ov)
{
    return 
        (start_square & 0x7FUL)|
        ((end_square & 0x7FUL) << 7)|
        ((promotion_piece & 0xFUL) << 14) |
        ((capture & 0x1UL) << 18)|
        ((castle & 0x1UL) << 19)|
        ((enpassant & 0x1UL) << 20)|
        ((castle_ov & 0xFUL) << 21);
}

static inline uint32_t set_heuristic(uint32_t move, uint32_t new_heristic) {return (move & 0x1FFFFFFUL) | ((new_heristic & 0x7FUL) << 25);}

static inline uint32_t start_square(uint32_t move) {return (move) & 0x7FUL;}

static inline uint32_t end_square(uint32_t move) {return (move >> 7) & 0x7FUL;}

static inline uint32_t promotion_piece(uint32_t move) {return (move >> 14) & 0xFUL;}

static inline uint32_t capture_flag(uint32_t move) {return (move >> 18) & 0x1UL;}

static inline uint32_t castle_flag(uint32_t move) {return (move >> 19) & 0x1UL;}

static inline uint32_t enpassant_flag(uint32_t move) {return (move >> 20) & 0x1UL;}

static inline uint32_t castle_overrides(uint32_t move) {return (move >> 21) & 0xFUL;}

static inline bool get_bit(uint64_t board, int square) {return board & (1ULL << square);}

static inline void push_bit(uint64_t* board, int square) {*board |= (1ULL << square);}

static inline void pop_bit(uint64_t* board, int square) {*board &= ~(1ULL << square);}

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

static inline unsigned int count_bits(uint64_t test)
{
    int bitnumber = 0;
    while (test)
    {
        test &= test - 1;
        bitnumber++;
    }
    return bitnumber;
}

static inline unsigned int LSB_index(uint64_t test)
{
    return (test) ? count_bits((test &  -test) - 1) : -1;
}

#endif
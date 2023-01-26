#ifndef BitTools
#define BitTools
#include <bits/stdc++.h>
#include <string>
#include <sstream>

const uint64_t A_FILE = 72340172838076673ULL;

const uint64_t B_FILE = 144680345676153346;

const uint64_t G_FILE = 4629771061636907072;

const uint64_t H_FILE = 9259542123273814144ULL;




    // encoding guide (using uint32_t)
        // 0000 0000 0000 0111 1111  start square 
        // 0000 0011 1111 1000 0000  end square
        // 0011 1100 0000 0000 0000  promotion piece (0 for none)
        // 0100 0000 0000 0000 0000  capture flag
        // 1000 0000 0000 0000 0000  castling flag


uint32_t pack_move(uint32_t start_square, uint32_t end_square, uint32_t promotion_piece, uint32_t capture, uint32_t castle)
{
    return 
        (start_square & 0x7FUL)|
        ((end_square << 7) & 0x7FUL)|
        ((promotion_piece << 14) & 0xFUL)|
        ((capture << 18) & 0x1UL)|
        ((castle << 19) & 0x1UL);
}
uint32_t start_square(uint32_t move) {return (move) & 0x7FUL;}

uint32_t end_square(uint32_t move) {return (move >> 7) & 0x7FUL;}

uint32_t promotion_piece(uint32_t move) {return (move >> 14) & 0xFUL;}

uint32_t capture(uint32_t move) {return (move >> 18) & 0x1UL;}

uint32_t castle(uint32_t move) {return (move >> 19) & 0x1UL;}

bool get_bit(uint64_t board, int square) {return board & (1ULL << square);}

void push_bit(uint64_t* board, int square) {*board |= (1ULL << square);}

void pop_bit(uint64_t* board, int square) {*board &= ~(1ULL << square);}



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


#endif
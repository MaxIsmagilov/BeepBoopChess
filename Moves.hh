#ifndef Moves
#define Moves
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include "Board.hh"



class MoveManager
{
    // encoding guide (using uint32_t)
        // 0000 0000 0000 0111 1111  start square 
        // 0000 0011 1111 1000 0000  end square
        // 0011 1100 0000 0000 0000  promotion piece (0 for none)
        // 0100 0000 0000 0000 0000  capture flag
        // 1000 0000 0000 0000 0000  castling flag

private: 
    MoveManager() {} // static class
public:
    static uint32_t pack_move(uint32_t start_square, uint32_t end_square, uint32_t promotion_piece, uint32_t capture, uint32_t castle)
    {
        return 
            (start_square & 0x7F)|
            ((end_square << 7) & 0x7F)|
            ((promotion_piece << 14) & 0xF)|
            ((capture << 18) & 0x1)|
            ((castle << 19) & 0x1);
    }
    static uint32_t start_square(uint32_t move) {return (move) & 0x7F;}
    static uint32_t end_square(uint32_t move) {return (move >> 7) & 0x7F;}
    static uint32_t promotion_piece(uint32_t move) {return (move >> 14) & 0xF;}
    static uint32_t capture(uint32_t move) {return (move >> 18) & 0x1;}
    static uint32_t castle(uint32_t move) {return (move >> 19) & 0x1;}
};





#endif
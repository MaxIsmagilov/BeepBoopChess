#ifndef Moves
#define Moves
#define U16 uint16_t
#define U32 unsigned int
#define U64 unsigned long long
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include "Board.hh"

// encoding guide (using U32)
    // 0000 0000 0000 0111 1111  start square 
    // 0000 0011 1111 1000 0000  end square
    // 0011 1100 0000 0000 0000  promotion piece (0 for none)
    // 0100 0000 0000 0000 0000  capture flag
    // 1000 0000 0000 0000 0000  castling flag

class MoveManager
{
private: 
    MoveManager() {} // static class
public:
    static U32 pack_move(U32 start_square, U32 end_square, U32 promotion_piece, U32 capture, U32 castle)
    {
        return 
            (start_square & 0x7F)|
            ((end_square << 7) & 0x7F)|
            ((promotion_piece << 14) & 0xF)|
            ((capture << 18) & 0x1)|
            ((castle << 19) & 0x1);
    }
    static U32 start_square(U32 move) {return (move) & 0x7F;}
    static U32 end_square(U32 move) {return (move >> 7) & 0x7F;}
    static U32 promotion_piece(U32 move) {return (move >> 14) & 0xF;}
    static U32 capture(U32 move) {return (move >> 18) & 0x1;}
    static U32 castle(U32 move) {return (move >> 19) & 0x1;}
}

#endif
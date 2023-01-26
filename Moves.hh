#ifndef Moves
#define Moves
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include "Board.hh"
#include "Tools.hh"

// pawn attack mask
uint64_t pawn_attacks[2][64];
uint64_t knight_attacks[64];

uint64_t pawn_mask(int square, int side) // side is 1 for white, -1 for black
{
    uint64_t att = 0ULL;            // attacks
    uint64_t bitboard = 0ULL;       // pawn square
    push_bit(&bitboard, square); 
    if (side == 1)                  // white's attacks
    {
        att |= 
                ((bitboard & A_FILE) ? 0ULL : (bitboard >> 9))| 
                ((bitboard & H_FILE) ? 0ULL : (bitboard >> 7));
    }
    else                            // black's attacks
    {
        att |= 
                ((bitboard & A_FILE) ? 0ULL : (bitboard << 7))| 
                ((bitboard & H_FILE) ? 0ULL : (bitboard << 9));
    }
    return att;
}
uint64_t knight_mask(int square) 
{
    uint64_t att = 0ULL;            // attacks
    uint64_t bitboard = 0ULL;       // knight square
    push_bit(&bitboard, square);
    //generate 17 and -15 attacks
    if (!(bitboard & A_FILE))
    {att |= (bitboard >> 17) | (bitboard << 15);}
    //generate -17 and 15 attacks
    if (!(bitboard & H_FILE))
    {att |= (bitboard << 17) | (bitboard >> 15);}
    //generate 10 and -6 attacks
    if (!((bitboard & A_FILE) || (bitboard & B_FILE)))
    {att |= (bitboard >> 10) | (bitboard << 6);}
    //generate -10 and 6 attacks
    if (!((bitboard & H_FILE) || (bitboard & G_FILE)))
    {att |= (bitboard << 10) | (bitboard >> 6);}
    return att;
}


void initialize_non_sliders() 
{
    for (int i = 0; i < 64; i++)
    {
        pawn_attacks[0][i] == pawn_mask(i,-1);
        pawn_attacks[1][i] == pawn_mask(i,1);
        knight_attacks[i] == knight_mask(i);
    }
}

#endif
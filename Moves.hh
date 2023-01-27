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
uint64_t king_attacks[64];

// it's what the mask is
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
uint64_t king_mask(int square)
{
    uint64_t att = 0ULL;            // attacks
    uint64_t bitboard = 0ULL;       // king square
    push_bit(&bitboard, square);
    if(!(bitboard & H_FILE))
    {
        att |= 
            (bitboard << 9)|
            (bitboard << 1)|
            (bitboard >> 7);
    }
    if(!(bitboard & A_FILE))
    {
        att |= 
            (bitboard >> 9)|
            (bitboard >> 1)|
            (bitboard << 7);
    }
    att |= (bitboard << 8) | (bitboard >> 8);
    return att;
}

uint64_t bishop_mask(int square)
{
    int rank, file;
    int t_rank = square / 8;
    int t_file = square % 8;
    uint64_t att = 0ULL;
    for (rank = t_rank + 1, file = t_file + 1; rank <= 6 && file <= 6; rank++, file++) 
        att |= 1ULL << (rank * 8 + file);
    for (rank = t_rank - 1, file = t_file + 1; rank >= 1 && file <= 6; rank--, file++)
        att |= 1ULL << (rank * 8 + file);
    for (rank = t_rank + 1, file = t_file - 1; rank <= 6 && file >= 1; rank++, file--) 
        att |= 1ULL << (rank * 8 + file);
    for (rank = t_rank - 1, file = t_file - 1; rank >= 1 && file >= 1; rank--, file--) 
        att |= 1ULL << (rank * 8 + file);
    return att;
}
uint64_t rook_mask(int square)
{
    int rank, file;
    int t_rank = square / 8;
    int t_file = square % 8;
    uint64_t att = 0ULL;
    for (rank = t_rank + 1; rank <= 6; rank++) 
        att |= 1ULL << (rank * 8 + t_file);
    for (rank = t_rank - 1; rank >= 1; rank--) 
        att |= 1ULL << (rank * 8 + t_file);
    for (file = t_file + 1; file <= 6; file++) 
        att |= 1ULL << (t_rank * 8 + file);
    for (file = t_file - 1; file >= 1; file--) 
        att |= 1ULL << (t_rank * 8 + file);
    return att;
}

uint64_t otf_bishop_attacks(int square, uint64_t soft_blockers, uint64_t hard_blockers)
{
    int rank, file;
    int t_rank = square / 8;
    int t_file = square % 8;
    uint64_t att = 0ULL;
    for (rank = t_rank + 1, file = t_file + 1; rank <= 7 && file <= 7; rank++, file++) 
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    for (rank = t_rank - 1, file = t_file + 1; rank >= 0 && file <= 7; rank--, file++)
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    for (rank = t_rank + 1, file = t_file - 1; rank <= 7 && file >= 0; rank++, file--) 
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    for (rank = t_rank - 1, file = t_file - 1; rank >= 0 && file >= 0; rank--, file--) 
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    return att;
}

uint64_t otf_rook_attacks(int square, uint64_t soft_blockers, uint64_t hard_blockers)
{
    int rank, file;
    int t_rank = square / 8;
    int t_file = square % 8;
    uint64_t att = 0ULL;
    for (rank = t_rank + 1, file = t_file; rank <= 7; rank++) 
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    for (rank = t_rank - 1, file = t_file; rank >= 0; rank--)
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    for (rank = t_rank, file = t_file + 1; file <= 7; file++) 
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    for (rank = t_rank, file = t_file - 1; file >= 0; file--) 
    {
        if ((1ULL << (rank * 8 + file)) & hard_blockers) break; 
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & soft_blockers) break;
    }
    return att;
}

void initialize_non_sliders() 
{
    for (int i = 0; i < 64; i++)
    {
        pawn_attacks[0][i] = pawn_mask(i,-1);
        pawn_attacks[1][i] = pawn_mask(i,1);
        knight_attacks[i] = knight_mask(i);
        king_attacks[i] = king_mask(i);
    }
}

#endif
#ifndef Moves
#define Moves
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include <vector>
#include "Board.hh"
#include "Tools.hh"
#include "Eval.hh"
#include "Random.hh"
#include "MagicNumbers.hh"
#include <cstring>
#include <algorithm>


enum rook_or_bishop {mROOK, mBISHOP};

// non-slider attack masks
uint64_t pawn_attacks[2][64]; // 0 is black
uint64_t knight_attacks[64];
uint64_t king_attacks[64];

// pawn silent moves
uint64_t silent_pawn_moves[2][64]; // 0 is black

uint64_t bishop_attack_masks[64];
uint64_t rook_attack_masks[64];

// rook and bishop attack arrays
uint64_t magic_rook_attacks   [64][4096];
uint64_t magic_bishop_attacks [64][512];

int bishop_occupancy_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

int rook_occupancy_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

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

// masks for magic generation
uint64_t bishop_mask(int square)
{
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

uint64_t rook_mask(int square)
{
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

// silent pawn moves
uint64_t silent_pawn_mask(int square, int side)
{
    uint64_t mv = 0ULL;
    uint64_t bitboard = 0ULL;       // pawn square
    push_bit(&bitboard, square); 
    if (side == 1)
    {
        mv |= (bitboard >> 8);
        if (square / 8 == 6) mv |=  (bitboard >> 16);
    }
    else
    {
        mv |= (bitboard << 8);
        if (square / 8 == 1) mv |=  (bitboard << 16);
    }
    return mv;
}

// on the fly attacks
uint64_t otf_bishop_attacks(int square, uint64_t blockers)
{
    int rank, file;
    int t_rank = square / 8;
    int t_file = square % 8;
    uint64_t att = 0ULL;
    for (rank = t_rank + 1, file = t_file + 1; rank <= 7 && file <= 7; rank++, file++) 
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    for (rank = t_rank - 1, file = t_file + 1; rank >= 0 && file <= 7; rank--, file++)
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    for (rank = t_rank + 1, file = t_file - 1; rank <= 7 && file >= 0; rank++, file--) 
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    for (rank = t_rank - 1, file = t_file - 1; rank >= 0 && file >= 0; rank--, file--) 
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    return att;
}

uint64_t otf_rook_attacks(int square, uint64_t blockers)
{
    int rank, file;
    int t_rank = square / 8;
    int t_file = square % 8;
    uint64_t att = 0ULL;
    for (rank = t_rank + 1, file = t_file; rank <= 7; rank++) 
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    for (rank = t_rank - 1, file = t_file; rank >= 0; rank--)
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    for (rank = t_rank, file = t_file + 1; file <= 7; file++) 
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    for (rank = t_rank, file = t_file - 1; file >= 0; file--) 
    {
        att |= 1ULL << (rank * 8 + file);
        if ((1ULL << (rank * 8 + file)) & blockers) break;
    }
    return att;
}

// occupancies
uint64_t get_occupancy(int index, int bit_count, uint64_t mask)
{
    uint64_t occupancy = 0ULL;

    for (int i = 0; i < bit_count; i++)
    {
        int square = LSB_index(mask);
        pop_bit(&mask, square);
        if (index & (1ULL << i))
        {
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}


// find magic
uint64_t find_magic(int square, int occupancy, int piece)
{
    uint64_t occupancies[4096];
    uint64_t attacks[4096];
    uint64_t checked_attacks[4096];

    uint64_t attack_mask = piece ? bishop_mask(square) : rook_mask(square);

    int occupancy_index = 1 << occupancy;

    for (int i = 0; i < occupancy_index; i++)
    {
        occupancies[i] = get_occupancy(i, occupancy , attack_mask);

        attacks[i] = piece ? otf_bishop_attacks(square, occupancies[i]) : otf_rook_attacks(square, occupancies[i]);
    }

    for (int i = 0; i < 10000000; i++)
    {
        uint64_t candidate = magic_candidate();
        if (count_bits((attack_mask * candidate) & 0xFF00000000000000) < 6) continue;
        memset(checked_attacks, 0ULL, sizeof(checked_attacks));

        int index, fail;
        for (index = 0, fail = 0; !fail && index < occupancy_index; index++)
        {
            int magic_index = (occupancies[index] * candidate) >> (64 - occupancy);

            if (checked_attacks[magic_index] == 0ULL) 
                checked_attacks[magic_index] = attacks[index];

            else if (checked_attacks[magic_index] != attacks[index])
                fail = 1;
        }

        if (!fail)
            return candidate;
    }
    return 0;
}

// init routines
void initialize_magic_numbers()
{
    for (int i = 0; i < 64; i++)
    {
        rook_magics[i] = find_magic(i, rook_occupancy_bits[i], mROOK);
    }
    for (int i = 0; i < 64; i++)
    {
       bishop_magics[i] = find_magic(i, bishop_occupancy_bits[i], mBISHOP);
    }
}

void initialize_non_sliders() 
{
    for (int i = 0; i < 64; i++)
    {
        pawn_attacks[0][i] = pawn_mask(i,-1);
        pawn_attacks[1][i] = pawn_mask(i,1);
        knight_attacks[i] = knight_mask(i);
        king_attacks[i] = king_mask(i);
        silent_pawn_moves[0][i] = silent_pawn_mask(i, -1);
        silent_pawn_moves[1][i] = silent_pawn_mask(i, 1);
    }
}

void initialize_sliders()
{
    for (int i = 0; i < 64; i++)
    {
        bishop_attack_masks[i] = bishop_mask(i);
        rook_attack_masks[i] = rook_mask(i);

        uint64_t b_attack_mask = bishop_attack_masks[i];
        uint64_t r_attack_mask = rook_attack_masks[i];

        int b_bits = count_bits(b_attack_mask);
        int r_bits = count_bits(r_attack_mask);

        int b_occi = 1 << b_bits;
        int r_occi = 1 << r_bits;

        for (int index = 0; index < b_occi; index++)
        {
            uint64_t occ = get_occupancy(index, b_bits, b_attack_mask);

            int magic_index = (occ * bishop_magics[i]) >> (64 - bishop_occupancy_bits[i]);

            magic_bishop_attacks[i][magic_index] = otf_bishop_attacks(i , occ);
        } 

        for (int index = 0; index < r_occi; index++)
        {
            uint64_t occ = get_occupancy(index, r_bits, r_attack_mask);

            int magic_index = (occ * rook_magics[i]) >> (64 - rook_occupancy_bits[i]);

            magic_rook_attacks[i][magic_index] = otf_rook_attacks(i , occ);
        }
    }
}

// magic attack calculator
static inline uint64_t get_bishop_attacks(int square, uint64_t occupancy)
{
    occupancy &= bishop_attack_masks[square];
    occupancy *= bishop_magics[square];
    occupancy >>= 64 - bishop_occupancy_bits[square];
    return magic_bishop_attacks[square][occupancy];
}

static inline uint64_t get_rook_attacks(int square, uint64_t occupancy)
{
    occupancy &= rook_attack_masks[square];
    occupancy *= rook_magics[square];
    occupancy >>= 64 - rook_occupancy_bits[square];
    return magic_rook_attacks[square][occupancy];
}

static inline uint64_t get_queen_attacks(int square, uint64_t occupancy)
{
    return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
}

// is attacked routine
static bool is_attacked(int square, Board* bd, int by_side)
{
    if (by_side == 1)
    {
        if (get_bishop_attacks(square, bd->all()) & ((bd->white[2]) | bd->white[4])) return true;
        if (get_rook_attacks(square, bd->all()) & ((bd->white[3]) | bd->white[4])) return true;
        if (pawn_attacks[0][square] & bd->white[0]) return true;
        if (knight_attacks[square] & bd->white[1]) return true;
        if (king_attacks[square] & bd->white[5]) return true;
    }
    else 
    {
        if (get_bishop_attacks(square, bd->all()) & (bd->black[2] | bd->black[4])) return true;
        if (get_rook_attacks(square, bd->all()) & (bd->black[3] | bd->black[4])) return true;
        if (pawn_attacks[1][square] & bd->black[0]) return true;
        if (knight_attacks[square] & bd->black[1]) return true;
        if (king_attacks[square] & bd->black[5]) return true;
    }
    return false;
}

// in check routine
static bool in_check(Board* bd, int side)
{
    if (side == 1)
    {
        return is_attacked(LSB_index(bd->white[5]), bd, -side);
    }
    else
    {
        return is_attacked(LSB_index(bd->black[5]), bd, -side);
    }
    return false;
}

// generate legal moves
void get_moves(Board* bd, std::vector<unsigned int>* vec)
{
    // clear vector
    vec->clear();
    // initiallize constants
    uint64_t all_pieces = bd->all();
    int side = bd->side;
    int binary_side = (side == 1) ? 1 : 0;
    uint64_t attacking_occ = (side == 1) ? bd->whites() : bd->blacks();
    uint64_t defending_occ = (side == 1) ? bd->blacks() : bd->whites();
    uint64_t* attacking_pieces = (side == 1) ? bd->white : bd->black;
    uint64_t* defending_pieces = (side == 1) ? bd->black : bd->white;
    for (unsigned int i = 0; i < 64; i++)
    {
        // populate attack bitboard
        uint64_t bitboard = 1ULL << i;
        uint64_t att = 0ULL;
        char castle = 0b11;
        int piece = -1;
        if (bitboard & attacking_pieces[0]) 
        {
            att |= (pawn_attacks[binary_side][i] & (defending_occ | (1ULL << bd->enpassant))) | (silent_pawn_moves[binary_side][i] & ~all_pieces); 
            piece = 1;
        }
        else if (bitboard & attacking_pieces[1]) att |= knight_attacks[i];
        else if (bitboard & attacking_pieces[2]) att |= get_bishop_attacks(i , all_pieces);
        else if (bitboard & attacking_pieces[3]) 
        {
            att |= get_rook_attacks(i , all_pieces);
            if ((i % 8) == 0) castle &= 0b01;
            else if ((i % 8) == 0) castle &= 0b10;
        }
        else if (bitboard & attacking_pieces[4]) att |= get_queen_attacks(i , all_pieces);
        else if (bitboard & attacking_pieces[5]) 
        {
            att |= king_attacks[i];
            castle = 0b00;
        }
        else continue;

        // remove self-captures
        att &= ~attacking_occ;

        // convert attack bitboard to a vector of moves
        while (att)
        {
            unsigned int castles_ov = (binary_side) ? (0b11 | (castle << 2)) : (0b1100 | castle);
            unsigned int j = LSB_index(att);
            pop_bit(&att, j);
            unsigned int captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            unsigned int enpassantflg = (j == bd->enpassant) ? 0b1 : 0;
            uint32_t move = pack_move(i , j, 0UL, captureflg, 0UL, enpassantflg, castles_ov);


            // verify checks
            Board test_checks = Board();
            test_checks.copy_from(bd);
            test_checks.move(move);
            if (in_check(&test_checks, side)) {continue;}

            // calculate heristics
            unsigned int heuristic_value = 10;

            // check for promotions
            if (piece == 1 && ((j / 8 == 0) || (j / 8 == 7)))
            {
                for (unsigned int promotee = 1; promotee < 5; promotee++)
                {
                    move = pack_move(i , j, promotee, captureflg, 0UL, enpassantflg, castles_ov);
                    move = set_heuristic(move, (heuristic_value + (captureflg * 5) + (promotee * 2)));
                    vec->push_back(move);
                }
            }

            // add non-promotion moves
            else 
            {
                if (is_attacked(LSB_index(attacking_pieces[4]), bd, -side));
                move = set_heuristic(move, (heuristic_value + (enpassantflg) + (captureflg * 5)));
                vec->push_back(move);
            }
        }
    }

    // castling routine
    if (side == 1)
    {
        if (bd->castles & 0b1000)
        {
            if (!(is_attacked(_E1,bd,-1) || is_attacked(_F1,bd,-1) || is_attacked(_G1,bd,-1)) && !(all_pieces & (_F1 | _G1)))
            {
                vec->push_back(pack_move(_E1,_G1, 0U, 0U, 1U, 0U, 0b0011));
            }
        }
        if (bd->castles & 0b0100)
        {
            if (!(is_attacked(_E1,bd,-1) || is_attacked(_D1,bd,-1) || is_attacked(_C1,bd,-1)) && !(all_pieces & (_D1 | _C1 | _B1)))
            {
                vec->push_back(pack_move(_E1,_C1, 0U, 0U, 1U, 0U, 0b0011));
            }
        }
    }
    else
    {
        if (bd->castles & 0b0010)
        {
            if (!(is_attacked(_E8,bd,1) || is_attacked(_F8,bd,1) || is_attacked(_G8,bd,1)) && !(all_pieces & (_F8 | _G8)))
            {
                vec->push_back(pack_move(_E8,_G8, 0U, 0U, 1U, 0U, 0b1100));
            }
        }
        if (bd->castles & 0b0001)
        {
            if (!(is_attacked(_E8,bd,1) || is_attacked(_D8,bd,1) || is_attacked(_C1,bd,1)) && !(all_pieces & (_D8 | _C8 | _B8)))
            {
                vec->push_back(pack_move(_E8,_C8, 0U, 0U, 1U, 0U, 0b1100));
            }
        }
    }
    
    int vector_size = vec->size();

    // exit if empty
    if (vec->size() == 0) return;

    // sort and finalize heuristics
    int num_best = (3 > vector_size / 3) ? vector_size / 3 : 3;
    int num_killer = (3 > vector_size - num_best) ? vector_size - num_best : 3;

    std::sort(vec->begin(), vec->end(), std::greater<>());
    for (int i = 0; i < num_best; i++)
        vec->at(i) |= (1UL << 31);
    std::sort(vec->begin(), vec->end());
    for (int i = 0; i < num_killer; i++)
        vec->at(i) |= (1UL << 30);
    std::sort(vec->begin(), vec->end(), std::greater<>());
    
}


#endif
#ifndef Moves
#define Moves
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include <vector>
#include "Board.hh"
#include "Tools.hh"
#include "Random.hh"
#include <cstring>


enum rook_or_bishop {ROOK, BISHOP};


// non-slider attack masks
uint64_t pawn_attacks[2][64]; // 0 is black
uint64_t knight_attacks[64];
uint64_t king_attacks[64];

uint64_t bishop_attack_masks[64];
uint64_t rook_attack_masks[64];
// magic numbers
uint64_t bishop_magics[64] = {
    0x120605409102020ULL,
    0xc004100200411009ULL,
    0x426022401040310ULL,
    0x6008258104203009ULL,
    0x61104020005080ULL,
    0x1001100210048200ULL,
    0xa0042c0928081004ULL,
    0x802940405241044ULL,
    0x21264504044c0408ULL,
    0x8000202801490024ULL,
    0x81022108402800aULL,
    0x8a082000240ULL,
    0x50b1491140080000ULL,
    0x1008110121500106ULL,
    0x120950221000ULL,
    0x2180030368020820ULL,
    0x10802082020800ULL,
    0x20100288020080ULL,
    0x2e10080210801100ULL,
    0x88000222064060ULL,
    0x8104001211204000ULL,
    0x1001201010100ULL,
    0x41844206112001ULL,
    0x18024b6011008ULL,
    0x1040a210840182ULL,
    0x1002202082040404ULL,
    0x6004091030010a60ULL,
    0x40c00480c010110ULL,
    0x30030001200802ULL,
    0x1490010040240100ULL,
    0x4204008100480440ULL,
    0x8008434382010440ULL,
    0x100a122000402810ULL,
    0x101118800305008ULL,
    0xa1420240008180aULL,
    0x480400808008200ULL,
    0x40020220020080ULL,
    0x20004100888084ULL,
    0x12080a00800c0880ULL,
    0x220401000020a0ULL,
    0x2a20880441801000ULL,
    0x1041002000400ULL,
    0x80202028131001ULL,
    0x214200800810ULL,
    0x20640408200400ULL,
    0x1111005000082ULL,
    0x4044840400404410ULL,
    0x309852204810201ULL,
    0x3030090104a4000ULL,
    0x9818444404202800ULL,
    0xc00420722881034ULL,
    0x81080084044012ULL,
    0x883040008ULL,
    0x4408901010020ULL,
    0x210041000b21280ULL,
    0x2884148410420302ULL,
    0x2010421042208ULL,
    0x8112002108080440ULL,
    0x2030084908411000ULL,
    0x3000400100420200ULL,
    0x2010404110420880ULL,
    0x4010182004100098ULL,
    0x404048822480201ULL,
    0xd1110140100c1ULL
};
uint64_t rook_magics[64] = {
    0x80008810204000ULL,
    0x540029000c2e001ULL,
    0x12002010400a0080ULL,
    0x100210008041000ULL,
    0x200020010040820ULL,
    0x8300140001000a08ULL,
    0x80008002000100ULL,
    0x10006002c804100ULL,
    0x800800080204000ULL,
    0x340402000401000ULL,
    0xc008808010002000ULL,
    0x80801000800803ULL,
    0xc480800800040080ULL,
    0x2000200900508ULL,
    0x1009000100140200ULL,
    0x4302001081204204ULL,
    0x40088020408000ULL,
    0x8888040002004ULL,
    0x20010040201102ULL,
    0x205010008100020ULL,
    0x1010008001004ULL,
    0x803808004008200ULL,
    0x4000400020110c8ULL,
    0xa02020000804104ULL,
    0x4220401080008021ULL,
    0x9040400080802000ULL,
    0x4800204200108a00ULL,
    0xb0020aa00320040ULL,
    0x2008020040400400ULL,
    0xc0040080020080ULL,
    0x5021c400100208ULL,
    0x4840200004081ULL,
    0x4401004202002080ULL,
    0x400080400c802001ULL,
    0x28801008802000ULL,
    0x20a002192000840ULL,
    0x8800400800800ULL,
    0x2800040080800200ULL,
    0x4600025014001118ULL,
    0x520106082000104ULL,
    0x540004424848000ULL,
    0x200050004002ULL,
    0x400820010420020ULL,
    0x2e11001050008ULL,
    0x20010208a0004ULL,
    0x180040002008080ULL,
    0xd001005200210014ULL,
    0x8011680c2000cULL,
    0x11002080004100ULL,
    0x200401000200040ULL,
    0x2002200100144500ULL,
    0x2100088210300ULL,
    0x104008008000480ULL,
    0xb040080820080ULL,
    0x1080020108100400ULL,
    0x6002142100804200ULL,
    0x408441281022202ULL,
    0x2040408201002012ULL,
    0x20002009124101ULL,
    0x4201020820041001ULL,
    0x802002110040802ULL,
    0x3002000408811002ULL,
    0x100010208209044ULL,
    0x1000300440a2086ULL
};
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

void initialize_magic_numbers()
{
    for (int i = 0; i < 64; i++)
    {
        rook_magics[i] = find_magic(i, rook_occupancy_bits[i], ROOK);
    }
    for (int i = 0; i < 64; i++)
    {
       bishop_magics[i] = find_magic(i, bishop_occupancy_bits[i], BISHOP);
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

static bool is_attacked(int square, Board bd, int by_side)
{
    if (by_side == 1)
    {
        if (get_bishop_attacks(square, bd.all()) & ((bd.white[2]) | bd.white[4])) return true;
        if (get_rook_attacks(square, bd.all()) & ((bd.white[3]) | bd.white[4])) return true;
        if (pawn_attacks[0][square] & bd.white[0]) return true;
        if (knight_attacks[square] & bd.white[1]) return true;
        if (king_attacks[square] & bd.white[5]) return true;
    }
    else 
    {
        if (get_bishop_attacks(square, bd.all()) & (bd.black[2] | bd.black[4])) return true;
        if (get_rook_attacks(square, bd.all()) & (bd.black[3] | bd.black[4])) return true;
        if (pawn_attacks[1][square] & bd.black[0]) return true;
        if (knight_attacks[square] & bd.black[1]) return true;
        if (king_attacks[square] & bd.black[5]) return true;
    }
    return false;
}

#endif
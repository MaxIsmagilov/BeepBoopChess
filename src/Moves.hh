#ifndef Moves
#define Moves

#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <algorithm>

#include "Board.hh"
#include "Tools.hh"
#include "Eval.hh"
#include "Random.hh"
#include "MagicNumbers.hh"

#define arradd(a, b, c) a[b] = c; b++;

const int FAIL_CUT = 1;

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
    bitboard = push_bit(bitboard, square); 
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
    bitboard = push_bit(bitboard, square);
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
    bitboard = push_bit(bitboard, square);
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
    bitboard = push_bit(bitboard, square); 
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
        mask = pop_bit(mask, square);
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
#define is_attacked(square, bd, attacking_side)                                     \
    (square == -1) ? false :  (                                                     \
    (attacking_side == 1) ?                                                         \
        (pawn_attacks[0][square] & bd.board[0]) ||                                  \
        (knight_attacks[square] & bd.board[1]) ||                                   \
        (get_bishop_attacks(square, all(bd)) & ((bd.board[2]) | bd.board[4])) ||    \
        (get_rook_attacks(square, all(bd)) & ((bd.board[3]) | bd.board[4])) ||      \
        (king_attacks[square] & bd.board[5])                                        \
    :                                                                               \
        (pawn_attacks[1][square] & bd.board[6]) ||                                  \
        (knight_attacks[square] & bd.board[7]) ||                                   \
        (get_bishop_attacks(square, all(bd)) & ((bd.board[8]) | bd.board[10])) ||   \
        (get_rook_attacks(square, all(bd)) & ((bd.board[9]) | bd.board[10])) ||     \
        (king_attacks[square] & bd.board[11]))                                      \

Board tst = Board();
int attacks[6];

static inline int heuristic_score(const Board& bd, unsigned int move)
{
    copy_from(&tst, bd);
    movef(&tst, {move,0});
    uint64_t* attacking_set = tst.board + (tst.side == 1) * 6;
    uint64_t defending_occ = (tst.side == -1) ? blacks(tst) : whites(tst);
    int LVA = -1;
    return 0;
}

// in check routine
#define in_check(bd, side_in_check) ((side_in_check == 1) ? is_attacked(LSB_index(bd.board[5]), bd, -side_in_check) : is_attacked(LSB_index(bd.board[11]), bd, -side_in_check))

static inline bool check_check(const Board& bd, unsigned int move)
{
    copy_from(&tst,bd);
    movef(&tst, {move,0});
    return in_check(tst, -tst.side);
}

// generate legal moves
static inline unsigned int get_moves(const Board& bd, moveWrapper* start_pointer)
{
    // clear array
    unsigned int size = 0;
    for (int i = size; i < ARRAY_SIZE; i++)
    {
        start_pointer[i] = {0,0};
    }

    // initiallize constants
    uint64_t all_pieces = all(bd);
    int side = bd.side;
    int binary_side = (side == 1) ? 1 : 0;
    uint64_t attacking_occ = (side == 1) ? whites(bd) : blacks(bd);
    uint64_t defending_occ = (side == 1) ? blacks(bd) : whites(bd);

    // set pointers for attacking and defending pieces
    const uint64_t* attacking_pieces = &(bd.board[0]) + (side == -1) * 6;
    const uint64_t* defending_pieces = &(bd.board[0]) + (side ==  1) * 6;

    // generate pawn moves
    uint64_t piece_board = attacking_pieces[PAWN];
    uint64_t att = 0ULL;
    while (piece_board)
    {
        int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= (pawn_attacks[binary_side][square] & (defending_occ | (1ULL << bd.enpassant))) | (silent_pawn_moves[binary_side][square] & ~all_pieces);
        att &= ~attacking_occ;
        while (att)
        {
            int j = LSB_index(att);
            att = pop_bit(att, j);
            int pp_flag = (abs(square-j) == 16) ? 0b1 : 0b0;
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            uint16_t enpassantflg = (j == bd.enpassant) ? 0b1 : 0;
            moveWrapper move = pack_move(square, j, (uint32_t) PAWN , 0U, captureflg, 0U, enpassantflg, pp_flag, 0b1111U);



            // calculate heristics
            uint16_t heuristic_value = 10U;

            // check for promotions
            if ((j / 8 == 0) || (j / 8 == 7))
            {
                for (uint32_t promotee = 1UL; promotee < 5UL; promotee++)
                {
                    move = pack_move(square , j, (uint32_t) PAWN , promotee, captureflg, 0U, enpassantflg, pp_flag, 0b1111U);
                    set_heuristic(move, (heuristic_value + (captureflg * 2) + (promotee * 4)));
                    arradd(start_pointer, size, move);
                }
            }
            else 
            {
                set_heuristic(move, (heuristic_value + (enpassantflg) + (captureflg * 5) ));
                arradd(start_pointer, size, move);
            }
        }
    }
            
    // generate knight moves
    piece_board = attacking_pieces[KNIGHT];
    att = 0ULL;
    while (piece_board)
    {
        int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= knight_attacks[square];
        att &= ~attacking_occ;
        while (att)
        {
            int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            moveWrapper move = pack_move(square , j, (uint32_t) KNIGHT, 0UL, captureflg, 0UL, 0UL, 0UL , 0b1111UL);

            // calculate heristics
            uint16_t heuristic_value = 10U;

            
            set_heuristic(move, (heuristic_value + (captureflg * 5) ));
            arradd(start_pointer, size, move);
        }
    }

    // generate bishop moves
    piece_board = attacking_pieces[BISHOP];
    att = 0ULL;
    while (piece_board)
    {
        int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= get_bishop_attacks(square, all_pieces);
        att &= ~attacking_occ;
        while (att)
        {
            int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            moveWrapper move = pack_move(square , j, (uint32_t) BISHOP ,0UL, captureflg, 0UL, 0UL,0UL ,0b1111UL);



            // calculate heristics
            uint16_t heuristic_value = 10U;

            
            set_heuristic(move, (heuristic_value + (captureflg * 5) ));
            arradd(start_pointer, size, move);
        }
    }

    // generate rook moves
    piece_board = attacking_pieces[ROOK];
    att = 0ULL;
    while (piece_board)
    {
        int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= get_rook_attacks(square, all_pieces);
        att &= ~attacking_occ;

        // get castling rights
        int castle = 0b11;
        if ((square % 8) == 0) castle &= 0b10;
        else if ((square % 8) == 0) castle &= 0b01;
        uint32_t castles_ov = (binary_side) ? (0b11 | (castle << 2)) : (0b1100 | castle);

        while (att)
        {
            int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            moveWrapper move = pack_move(square , j, (uint32_t) ROOK, 0UL ,captureflg, 0UL, 0UL, 0UL , castles_ov);


            // calculate heristics
            uint16_t heuristic_value = 10U;

            
            set_heuristic(move, (heuristic_value + (captureflg * 5) ));
            arradd(start_pointer, size, move);
        }
    }

    // generate queen moves
    piece_board = attacking_pieces[QUEEN];
    att = 0ULL;
    while (piece_board)
    {
        int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= get_queen_attacks(square, all_pieces);
        att &= ~attacking_occ;
        while (att)
        {
            int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            moveWrapper move = pack_move(square , j, (uint32_t) QUEEN, 0UL ,captureflg, 0UL, 0UL, 0UL, 0b1111UL);


            // calculate heristics
            uint16_t heuristic_value = 10ULL;

            
            set_heuristic(move, (heuristic_value + (captureflg * 5) ));
            arradd(start_pointer, size, move);
        }
    }

    // generate king moves
    piece_board = attacking_pieces[KING];
    att = 0ULL;
    while (piece_board)
    {
        int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= king_attacks[square];
        att &= ~attacking_occ;

        // get castling rights
        uint32_t castles_ov = (binary_side) ? (0b0011) : (0b1100);

        while (att)
        {
            int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            moveWrapper move = pack_move(square , j, (uint32_t) KING, 0UL , captureflg, 0UL, 0UL, 0UL, castles_ov);

            // verify checks

            // calculate heristics
            uint64_t heuristic_value = 10ULL;

            
            set_heuristic(move, (heuristic_value + (captureflg * 5) ));
            arradd(start_pointer, size, move);
        }
    }


    // castling routine
    if (side == 1)
    {
        if (bd.castles & 0b1000)
        {
            if (!(is_attacked(_E1,bd,-1) || is_attacked(_F1,bd,-1) || is_attacked(_G1,bd,-1)) && !(all_pieces & (_F1 | _G1)))
            {
                arradd(start_pointer, size, pack_move(_E1,_G1, KING, 0U, 0U, 1U, 0U, 0U, 0b0011));
            }
        }
        if (bd.castles & 0b0100)
        {
            if (!(is_attacked(_E1,bd,-1) || is_attacked(_D1,bd,-1) || is_attacked(_C1,bd,-1)) && !(all_pieces & (_D1 | _C1 | _B1)))
            {
                arradd(start_pointer, size, pack_move(_E1,_C1, KING, 0U, 0U, 1U, 0U, 0U, 0b0011));
            }
        }
    }
    else
    {
        if (bd.castles & 0b0010)
        {
            if (!(is_attacked(_E8,bd,1) || is_attacked(_F8,bd,1) || is_attacked(_G8,bd,1)) && !(all_pieces & (_F8 | _G8)))
            {
                arradd(start_pointer, size, pack_move(_E8,_G8, KING, 0U, 0U, 1U, 0U, 0U, 0b1100));
            }
        }
        if (bd.castles & 0b0001)
        {
            if (!(is_attacked(_E8,bd,1) || is_attacked(_D8,bd,1) || is_attacked(_C1,bd,1)) && !(all_pieces & (_D8 | _C8 | _B8)))
            {
                arradd(start_pointer, size, pack_move(_E8,_C8, KING, 0U, 0U, 1U, 0U, 0U, 0b1100));
            }
        }
    }
    



    // exit if empty
    if (size == 0) return 0;

    // remove illegal moves
    for (int i = 0; i < size; i++)
    {
        if (check_check(bd, start_pointer[i]._mv)) start_pointer[i] = {0,0};
    }

    // sort and finalize heuristics
    int num_best = (FAIL_CUT > size / FAIL_CUT) ? size / FAIL_CUT : FAIL_CUT;
    int num_killer = (FAIL_CUT > size - num_best) ? size - num_best : FAIL_CUT;

    // sort moves
    std::sort(start_pointer, start_pointer + size, std::greater<>());
    for (int i = 0; i < num_best; i++)
        start_pointer[i]._hv |= (1UL << 15);
    std::sort(start_pointer, start_pointer + size);
    for (int i = 0; i < num_killer; i++)
        start_pointer[i]._hv |= (1UL << 14);
    std::sort(start_pointer, start_pointer + size, std::greater<>());
    return size;
}

moveWrapper pull_move(std::string mv, Board* bd)
{
    std::array<moveWrapper, 120> arr = std::array<moveWrapper, 120>();
    get_moves(*bd, arr.begin());

    unsigned int source = (mv[0] - 'a') + (8 - (mv[1] - '0')) * 8;
    unsigned int end = (mv[2] - 'a') + (8 - (mv[3] - '0')) * 8;
    unsigned int promotion = 0UL;
    if (mv.length() == 5) 
    {
        char pc = mv[4];
        if (pc == 'Q' || pc == 'q')
            promotion = 4UL; 
        if (pc == 'R' || pc == 'R')
            promotion = 3UL; 
        if (pc == 'B' || pc == 'b')
            promotion = 2UL; 
        if (pc == 'K' || pc == 'k')
            promotion = 1UL; 
    }
    for (moveWrapper i : arr)
    {
        if (start_square(i) == source && end_square(i) == end && promotion == promotion_piece(i))
            return i;
    }
    return {0,0};
}

void print_move(moveWrapper move)
{
    char sourcef = 'a' + (start_square(move) % 8);
    char sourcer = '8' - (start_square(move) / 8);
    char endf = 'a' + (end_square(move) % 8);
    char endr = '8' - (end_square(move) / 8);
    
    char piece;
    switch (promotion_piece(move))
    {
        case 0UL:
        printf("%c%c%c%c", sourcef, sourcer, endf, endr); return;
        case 1UL:
        piece = 'N'; break;
        case 2UL:
        piece = 'B'; break;
        case 3UL:
        piece = 'R'; break;
        case 4UL:
        piece = 'Q'; break;
    }
    printf("%c%c%c%c%c", sourcef, sourcer, endf, endr, piece);
}


void print_moves(const Board& bd)
{
    std::array<moveWrapper, 120> arr = std::array<moveWrapper, 120>();
    get_moves(bd, arr.begin());
    for (moveWrapper mw : arr)
    {
        unsigned int j = mw._mv;
        if (j == 0) break;
        print_move(mw);
        printf(", ");
    }
    printf("\n\t");
}


#endif
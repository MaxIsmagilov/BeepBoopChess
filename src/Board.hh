
#ifndef CHESS_UTILS
#define CHESS_UTILS

#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include "Random.hh"
#include "MagicNumbers.hh"

/*************************************\
|           TOOL DEFINITIONS          |
\*************************************/

/*      misc. constants, enums, and macroes*/

/// @brief the bits for the a-file
const uint64_t A_FILE = 72340172838076673ULL;

/// @brief the bits for the b-file
const uint64_t B_FILE = 144680345676153346ULL;

/// @brief the bits for the g-file
const uint64_t G_FILE = 4629771061636907072ULL;

/// @brief the bits for the h-file
const uint64_t H_FILE = 9259542123273814144ULL;

/// @brief gamestates (unused)
enum GAMESTATE {RUNNING, BLACK_CHECKMATE, STALEMATE, WHITE_CHECKMATE};

/// @brief piece enums
enum piece_numbers {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

/// @brief color enums
enum colors {WHITE, BLACK};

/// @brief squares of pieces
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

/// @brief utility enum for magics (mROOK, mBISHOP)
enum rook_or_bishop {mROOK, mBISHOP};

/// @brief white's bitboards
#define white(i) board[i]

/// @brief black's bitboards
#define black(i) board[i+6] 

/// @brief adds an element to an array
#define arradd(a, b, c) a[b] = c; b++;

/// @brief length of best and worst moves
constexpr int FAIL_CUT = 1;

/// @brief size of arrays used for move storage
constexpr unsigned int ARRAY_SIZE = 120;

/*      non-slider attack masks     */

/// @brief pawn attack table
/// @note 0 is black
uint64_t pawn_attacks[2][64]; 
/// @brief knight attack table
uint64_t knight_attacks[64];
/// @brief king attack table
uint64_t king_attacks[64];

/*       pawn silent moves          */

/// @brief pawn silent move table
/// @note 0 is black
uint64_t silent_pawn_moves[2][64]; // 

/*      occupancy masks             */

/// @brief occupancy masks for bishops
uint64_t bishop_attack_masks[64];
/// @brief occupancy masks for rooks
uint64_t rook_attack_masks[64];

/*      magic attack tables         */

/// @brief bishop attack table
uint64_t magic_bishop_attacks [64][512];
/// @brief rook attack table
uint64_t magic_rook_attacks   [64][4096];

/*      relevant occupancy bits     */

/// @brief occupancy bits for a square for a bishop
constexpr int bishop_occupancy_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};
/// @brief occupancy bits for a square for a rook
constexpr int rook_occupancy_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

/*      wrapper struct to store and sort moves      */

/// @brief struct to store moves and their heuristic
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


/*      move encoding guide (using uint32_t wrapper struct)         */

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

/*      uint32_t move bit unpacking macroes         */

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

/*      print bitboard (debug tool)     */

/// @brief prints a bitboard
/// @param bb 
/// @return a string
std::string print_bitboard(const uint64_t& bb)
{
    std::string str = "";
    for (int i = 0; i < 64; i++)
    {
        str += (get_bit(bb,i)) ? '1' : '0';
        if (i%8 == 7) {str += '\n';}
    }
    return str;
}

/*      counting and finding bits       */

/// @brief counts the bits in a bitboard
/// @param bitboard 
/// @return the number of bits
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

/// @brief  finds the index of the least significant bit
/// @param test 
/// @return the index
static inline int LSB_index(const uint64_t& test) 
{
    return (test) ? count_bits((test &  -test) - 1) : -1;
}

/*      pack moves      */

/// @brief packs a move_wrapper struct (1hv assumed)
/// @param start_square 
/// @param end_square 
/// @param moved_piece 
/// @param promotion_piece 
/// @param capture 
/// @param castle 
/// @param enpassant 
/// @param pp_flag 
/// @param castle_ov 
/// @return a move_wrapper
static inline move_wrapper pack_move(const unsigned int& start_square, const unsigned int& end_square, const unsigned int& moved_piece, 
                                    const unsigned int& promotion_piece, const unsigned int& capture, const unsigned int& castle, 
                                    const unsigned int&  enpassant, const unsigned int& pp_flag , const unsigned int& castle_ov) 
{
    return {((start_square & 0x3FUL)|((end_square & 0x3FUL) << 6) | ((moved_piece&0xFUL) << 12) | ((promotion_piece & 0xFUL) << 16) | ((capture & 0x1UL) << 20) | ((castle & 0x1UL) << 21)| ((enpassant & 0x1UL) << 22)|((pp_flag & 0x1UL) << 23)| ((castle_ov & 0xFUL) << 24)), 1};
}

/*************************************\
|           BOARD DEFINITION          |
\*************************************/


/*      board struct        */

/// @brief board struct with all relevant info
struct Board
{ 
public:
    uint64_t board[12];
    uint8_t castles = 0b00000000;       // represents castling ability in 0b0000KQkq
    int8_t side = 1;                    // side to move, 1 for white, -1 for black
    int8_t enpassant = -1;              // enpassant square (-1 for none)
    uint8_t halfmoves;                  // halfmoves
    uint8_t fullmoves;                  // fullmoves
};

/// @brief gets white's occupancy
/// @param bd 
/// @return bitboard of occupancy
static inline uint64_t whites(const Board& bd) {return bd.board[0] | bd.board[1] | bd.board[2] | bd.board[3] | bd.board[4]  | bd.board[5] ;}
/// @brief gets black's occuspancy
/// @param bd 
/// @return bitboard of occupancy
static inline uint64_t blacks(const Board& bd) {return bd.board[6] | bd.board[7] | bd.board[8] | bd.board[9] | bd.board[10] | bd.board[11];}
/// @brief gets all occupancy
/// @param bd 
/// @return bitboard of occupancy
static inline uint64_t all(const Board& bd) {return whites(bd) | blacks(bd);}

/// @brief loads a position from a FEN code
/// @param bd a pointer to a Board
/// @param FEN 
void import_FEN(Board* bd, const std::string& FEN)
{
    char arr[64];  
    int index = 0;
    int feni = 0;
    for (feni; FEN[feni] != ' '; feni++)
    {
        if (FEN[feni] == '/') {continue;}
        else if (std::isdigit(FEN[feni])) 
        {
            for (int i = 0; i <= (FEN[feni] - '1'); i++)
            {
                arr[index] = '\0';
                index++;
            }
        }
        else 
        {
            arr[index] = FEN[feni];
            index++;
        }
    }
    for (int i = 0; i < 12; i++) {bd->board[i] = 0ULL;}
    for (int pos = 0; pos < 64; pos++)
    {
        uint64_t itr = 1ULL << pos;
        switch (arr[pos])
        {
            case 'P': bd->board[0] |= itr; break;
            case 'N': bd->board[1] |= itr; break;
            case 'B': bd->board[2] |= itr; break;
            case 'R': bd->board[3] |= itr; break;
            case 'Q': bd->board[4] |= itr; break;
            case 'K': bd->board[5] |= itr; break;

            case 'p': bd->board[6] |= itr; break;
            case 'n': bd->board[7] |= itr; break;
            case 'b': bd->board[8] |= itr; break;
            case 'r': bd->board[9] |= itr; break;
            case 'q': bd->board[10] |= itr; break;
            case 'k': bd->board[11] |= itr; break;
        }
    }
    feni++;
    if (FEN[feni] == 'w') {bd->side = 1;} else {bd->side = -1;}
    feni += 2;
    bd->castles = 0;
    for (feni; FEN[feni] != ' '; feni++)
    {
        switch (FEN[feni])
        {
            case 'K': bd->castles |= 0b00001000; break;
            case 'Q': bd->castles |= 0b00000100; break;
            case 'k': bd->castles |= 0b00000010; break;
            case 'q': bd->castles |= 0b00000001; break;
        }
    }
    feni++;
    if (FEN[feni] != '-')
    {
        bd->enpassant = 0;
        bd->enpassant += FEN[feni] - 'a';
        feni++;
        bd->enpassant += 8 * (7 - (FEN[feni] - '1'));
    }
    else {bd->enpassant = -1;}
    feni += 2;
    bd->fullmoves = 0;
    bd->halfmoves = 0;
    do
    {
        bd->halfmoves *= 10;
        bd->halfmoves += FEN[feni] - '0';
    } while (isdigit(FEN[++feni]));
    feni++;
    do
    {
        bd->fullmoves *= 10;
        bd->fullmoves += FEN[feni] - '0';
    } while (isdigit(FEN[++feni]));
}

/// @brief nicely formats a board
/// @param bd a pointer
/// @return a string of the board
std::string print_board(Board* bd)
{
    std::string boardstr = "--|---|---|---|---|---|---|--\n";
    const char p[12] = {'P','N','B','R','Q','K','p','n','b','r','q','k'};
    for (int pos = 0; pos < 64; pos++)
    {
        uint64_t itr = std::pow(2,pos);
        int piece;
        for (piece = 0; piece < 12; piece++)
        {
            if (bd->board[piece] & itr) {boardstr += p[piece]; break;}
        }
        if (piece != 6) {;}
        else if ((pos + (pos / 8)) % 2 == 1) {boardstr += '.';}
        else {boardstr += ' ';}
        
        if (pos % 8 == 7) boardstr += "\n--|---|---|---|---|---|---|--\n";
        else boardstr += " | ";
        
    }

    return boardstr;
}

/// @brief copies a board
/// @param target a pointer to a Board
/// @param origin a Board to be copied
const static inline void copy_board(Board* target, const Board& origin)
{
    memcpy(target, &(origin), sizeof(Board));
}

/// @brief makes a move
/// @param bd a Board object
/// @param move a movewrapper
const static inline void movef(Board& bd, const move_wrapper& move)
{
    
    bd.halfmoves++;
    if (capture_flag(move)) bd.halfmoves = 0;
    const int end = end_square(move);
    const int start = start_square(move);
    const int moving_side = (bd.side == 1) ? 0 : 6;
    bd.side = -(bd.side);
    if (bd.side == 1) bd.fullmoves++;
    bd.castles &= castle_overrides(move);
    bd.enpassant = -1;
    if (moved_piece(move) == 0)
    {
        if (double_pawn_flag(move)) bd.enpassant = start - 8;
        bd.halfmoves = 0;
    }
    else if (moved_piece(move) == 0)
    {
        if (double_pawn_flag(move)) bd.enpassant = start + 8;
        bd.halfmoves = 0;
    }
    bd.board[moving_side + moved_piece(move)] &= ~(1ULL << start);
    if (promotion_piece(move))
    {
        bd.board[moving_side + promotion_piece(move)] |= 1ULL << end;
    }
    else
    {
        bd.board[moving_side + moved_piece(move)] |= 1ULL << end;
    }
    if (castle_flag(move)) 
    {
        if (moving_side)    
        {
            bd.white(3) = push_bit(bd.white(3), ((end + start) / 2));
            bd.white(3) = pop_bit(bd.white(3), ((end > start) ? _H1 : _A1));
        }
        else                
        {
            bd.black(3) = push_bit(bd.black(3), ((end + start) / 2));
            bd.black(3) = pop_bit(bd.black(3), ((end > start) ? _H8 : _A8));
        }
    }
    if (enpassant_flag(move)) 
    {
        bd.white(0) &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
        bd.black(0) &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
    }

}

/// @brief converts the board to a string
/// @param bd 
/// @return a string version of the board
std::string to_string(Board* bd)
{
    std::string bdstr = "";
    char p[12] = {'P','N','B','R','Q','K','p','n','b','r','q','k'};
    for (int pos = 0; pos < 64; pos++)
    {
        uint64_t itr = std::pow(2,pos);
        int piece;
        for (piece = 0; piece < 12; piece++)
        {
            if (bd->board[piece] & itr) {bdstr += p[piece]; break;}
        }
        if (piece != 12) ;
        else if ((pos + (pos / 8)) % 2 == 1) {bdstr += '.';}
        else {bdstr += ' ';}
        
        if (pos % 8 == 7) bdstr += "\n";
        else bdstr += " ";
        
    }
    bdstr += "side to move: ";
    if (bd->side == 1) bdstr += "white"; else bdstr += "black";
    bdstr += "\ncastling ability: ";
    if (bd->castles & 0b00001000) bdstr += "K";
    if (bd->castles & 0b00000100) bdstr += "Q";
    if (bd->castles & 0b00000010) bdstr += "k";
    if (bd->castles & 0b00000001) bdstr += "q";
    bdstr += "\nenpassant ability: " + std::to_string(bd->enpassant);
    bdstr += "\nmove #" + std::to_string(bd->fullmoves);
    bdstr += "\nhalfmoves: " + std::to_string(bd->halfmoves);
    bdstr += "\n";


    return bdstr;
}


/*************************************\
|           MOVE GENERATION           |
\*************************************/


/*      attack masks        */

/// @brief find an attack mask for a pawn
/// @param square 
/// @param side 1 for white, -1 for black
/// @return a bitboard mask
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

/// @brief find a mask for a knight
/// @param square 
/// @return a bitboard mask
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

/// @brief find a mask for a king
/// @param square 
/// @return a bitboard mask
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

/// @brief find a mask for bishop occupancies
/// @param square 
/// @return a bitboard mask
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

/// @brief find a mask for rook occupancies
/// @param square 
/// @return a bitboard mask
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

/// @brief silent masks for pawns
/// @param square 
/// @param side 
/// @return a bitboard mask
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

/*      on the fly attacks and other magic generation tools     */

/// @brief on the fly attacks for bishops 
/// @param square 
/// @param blockers 
/// @return an attack bitboard
/// @note for magic generation 
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

/// @brief on the fly attacks for rooks 
/// @param square 
/// @param blockers 
/// @return an attack bitboard
/// @note for magic generation 
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

/// @brief gets the occupancy for a mask
/// @param index 
/// @param bit_count 
/// @param mask 
/// @return a bitboard
/// @note for magic generation
uint64_t get_occupancy(int index, int bit_count, uint64_t mask)
{
    uint64_t occupancy = 0ULL;

    for (int i = 0; i < bit_count; i++)
    {
        const int square = LSB_index(mask);
        mask = pop_bit(mask, square);
        if (index & (1ULL << i))
        {
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}

/// @brief finds a magic number
/// @param square 
/// @param occupancy 
/// @param piece 
/// @return a magic number
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

/*      initialization routines         */

/// @brief initalizes magic numbers 
/// @note does not do anything, magics are already generated and known at compile-time
void initialize_magic_numbers()
{
    for (int i = 0; i < 64; i++)
    {
        //rook_magics[i] = find_magic(i, rook_occupancy_bits[i], mROOK);
    }
    for (int i = 0; i < 64; i++)
    {
       //bishop_magics[i] = find_magic(i, bishop_occupancy_bits[i], mBISHOP);
    }
}

/// @brief initialize attack tables for non-slider pieces (pawns, knights, kings)
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

/// @brief initialize attack tables for slider pieces (bishops, rooks, queens)
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

/*      slider attack lookups       */

/// @brief gets bishop attacks for an occupancy
/// @param square 
/// @param occupancy 
/// @return an attack bitboard
static inline uint64_t get_bishop_attacks(const int square, uint64_t occupancy)
{
    occupancy &= bishop_attack_masks[square];
    occupancy *= bishop_magics[square];
    occupancy >>= 64 - bishop_occupancy_bits[square];
    return magic_bishop_attacks[square][occupancy];
}

/// @brief gets rook attacks for an occupancy
/// @param square 
/// @param occupancy 
/// @return an attack bitboard
static inline uint64_t get_rook_attacks(const int square, uint64_t occupancy)
{
    occupancy &= rook_attack_masks[square];
    occupancy *= rook_magics[square];
    occupancy >>= 64 - rook_occupancy_bits[square];
    return magic_rook_attacks[square][occupancy];
}

/// @brief gets queen attacks for an occupancy
/// @param square 
/// @param occupancy 
/// @return an attack bitboard
static inline uint64_t get_queen_attacks(const int square, const uint64_t& occupancy)
{
    uint64_t occ1 = occupancy;

    occ1 &= bishop_attack_masks[square];
    occ1 *= bishop_magics[square];
    occ1 >>= 64 - bishop_occupancy_bits[square];

    uint64_t occ2 = occupancy;

    occ2 &= rook_attack_masks[square];
    occ2 *= rook_magics[square];
    occ2 >>= 64 - rook_occupancy_bits[square];

    return magic_rook_attacks[square][occ2] | magic_bishop_attacks[square][occ1];
}

/*      check and attack routines       */

/// @brief checks if a square is attacked
/// @param square 
/// @param bd 
/// @param attacking_side 
/// @return whether the square is attacked or not
static inline const bool is_attacked(int square, const Board& bd, const int& attacking_side)
{                                  \
    return (square == -1) ? false :  (                                                     
                            (attacking_side == 1) ?                                                         
                                            (pawn_attacks[0][square] & bd.board[0]) ||                                  
                                            (knight_attacks[square] & bd.board[1]) ||                                   
                                            (get_bishop_attacks(square, all(bd)) & ((bd.board[2]) | bd.board[4])) ||    
                                            (get_rook_attacks(square, all(bd)) & ((bd.board[3]) | bd.board[4])) ||      
                                            (king_attacks[square] & bd.board[5])                                        
                                            :                                                                               
                                            (pawn_attacks[1][square] & bd.board[6]) ||                                  
                                            (knight_attacks[square] & bd.board[7]) ||                                   
                                            (get_bishop_attacks(square, all(bd)) & ((bd.board[8]) | bd.board[10])) ||   
                                            (get_rook_attacks(square, all(bd)) & ((bd.board[9]) | bd.board[10])) ||     
                                            (king_attacks[square] & bd.board[11]));                                  
}

/// @brief scores a move for move ordering
/// @param bd 
/// @param move 
/// @param tst 
/// @return the heuristic score (currently inused)
static inline int heuristic_score(const Board& bd, const unsigned int move, Board& tst)
{
    copy_board(&tst, bd);
    movef(tst, {move,0});
    uint64_t* attacking_set = tst.board + (tst.side == 1) * 6;
    uint64_t defending_occ = (tst.side == -1) ? blacks(tst) : whites(tst);
    int LVA = -1;
    return 0;
}

/// @brief checks if a side is in check
/// @param bd 
/// @param side_in_check 
/// @return whether the side is in check
static inline const bool in_check(const Board& bd, const int& side_in_check) 
{
    return ((side_in_check == 1) ? is_attacked(LSB_index(bd.board[5]), bd, -side_in_check) : is_attacked(LSB_index(bd.board[11]), bd, -side_in_check));
}

/// @brief checks whether a move would put the moving side in check
/// @param bd 
/// @param move 
/// @param tst 
/// @return boolean value
static inline const bool check_check(const Board& bd, const unsigned int move, Board& tst)
{
    copy_board(&tst,bd);
    movef(tst, {move,0});
    return in_check(tst, -tst.side);
}

/*      move generation routine     */

/// @brief generate all legal moves
/// @param bd 
/// @param start_pointer 
/// @return the size of the array
static inline unsigned int get_moves(const Board& bd, move_wrapper* start_pointer)
{
    // clear array
    unsigned int size = 0;

    memset(start_pointer, 0b0, sizeof(move_wrapper)*ARRAY_SIZE);

    // initiallize constants
    const uint64_t all_pieces = all(bd);
    const int side = bd.side;
    const int binary_side = (side == 1) ? 1 : 0;
    const uint64_t attacking_occ = (side == 1) ? whites(bd) : blacks(bd);
    const uint64_t defending_occ = (side == 1) ? blacks(bd) : whites(bd);

    // set pointers for attacking and defending pieces
    const uint64_t* attacking_pieces = &(bd.board[0]) + (side == -1) * 6;
    const uint64_t* defending_pieces = &(bd.board[0]) + (side ==  1) * 6;

    // generate pawn moves
    uint64_t piece_board = attacking_pieces[PAWN];
    uint64_t att = 0ULL;
    while (piece_board)
    {
        const int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= (pawn_attacks[binary_side][square] & (defending_occ | (1ULL << bd.enpassant))) | (silent_pawn_moves[binary_side][square] & ~all_pieces);
        att &= ~attacking_occ;
        while (att)
        {
            const int j = LSB_index(att);
            att = pop_bit(att, j);
            const int pp_flag = (abs(square-j) == 16) ? 0b1 : 0b0;
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            uint16_t enpassantflg = (j == bd.enpassant) ? 0b1 : 0;
            move_wrapper move = pack_move(square, j, (uint32_t) PAWN , 0U, captureflg, 0U, enpassantflg, pp_flag, 0b1111U);



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
        const int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= knight_attacks[square];
        att &= ~attacking_occ;
        while (att)
        {
            const int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            move_wrapper move = pack_move(square , j, (uint32_t) KNIGHT, 0UL, captureflg, 0UL, 0UL, 0UL , 0b1111UL);

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
        const int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= get_bishop_attacks(square, all_pieces);
        att &= ~attacking_occ;
        while (att)
        {
            const int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            move_wrapper move = pack_move(square , j, (uint32_t) BISHOP ,0UL, captureflg, 0UL, 0UL,0UL ,0b1111UL);



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
        const int square = LSB_index(piece_board);
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
            const int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            move_wrapper move = pack_move(square , j, (uint32_t) ROOK, 0UL ,captureflg, 0UL, 0UL, 0UL , castles_ov);


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
        const int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= get_queen_attacks(square, all_pieces);
        att &= ~attacking_occ;
        while (att)
        {
            const int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            move_wrapper move = pack_move(square , j, (uint32_t) QUEEN, 0UL ,captureflg, 0UL, 0UL, 0UL, 0b1111UL);


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
        const int square = LSB_index(piece_board);
        piece_board = pop_bit(piece_board, square);
        att |= king_attacks[square];
        att &= ~attacking_occ;

        // get castling rights
        uint32_t castles_ov = (binary_side) ? (0b0011) : (0b1100);

        while (att)
        {
            const int j = LSB_index(att);
            att = pop_bit(att, j);
            uint32_t captureflg = ((1ULL << j) & defending_occ) ? 0b1 : 0;
            move_wrapper move = pack_move(square , j, (uint32_t) KING, 0UL , captureflg, 0UL, 0UL, 0UL, castles_ov);

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
    Board tst = Board();
    std::for_each(start_pointer, start_pointer+size, [&](move_wrapper& mv) mutable 
        {
            copy_board(&tst,bd);
            movef(tst, mv);
            if (in_check(tst, -tst.side)) mv = {0,0};
        });

    // sort and finalize heuristics
    std::sort(start_pointer, start_pointer + ARRAY_SIZE, std::greater<>());
    const int final_size = std::find_if(start_pointer, start_pointer+ARRAY_SIZE, [](move_wrapper mv) -> bool {return (mv._mv != 0);}) - start_pointer;
    const auto num_best = std::min(final_size / FAIL_CUT , FAIL_CUT);
    const auto num_killer = std::min(final_size - num_best , FAIL_CUT);

    // set best and killer values
    std::for_each(start_pointer, start_pointer+num_best, 
        [&](move_wrapper& mv) mutable {  mv._hv |= (1UL << 15); } );
    std::for_each(start_pointer+(final_size-num_killer), start_pointer+final_size, 
        [&](move_wrapper& mv) mutable {  mv._hv |= (1UL << 14); } );
    std::swap_ranges(start_pointer+(final_size-num_killer), start_pointer+final_size, start_pointer+num_best);

    // return size of the array
    return final_size;
}

/*      move checking routine       */

/// @brief prints the move and heuristic values for a board
/// @param bd 
void print_move_values(const Board& bd)
{
    int i = 1;
    printf("\n\t");
    std::array<move_wrapper, 120> arr = std::array<move_wrapper, 120>();
    get_moves(bd, arr.begin());
    for (move_wrapper mw : arr)
    {
        unsigned int j = mw._mv;
        printf("%i:%x\t->%i\n\t", i++, j, mw._hv);
    }
    printf("\n");
}

#endif
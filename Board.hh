#ifndef ChessBoard
#define ChessBoard

class Board;

#include <bits/stdc++.h>
#include <sstream>
#include <cstring>

#include "Tools.hh"



Board* history = new Board[256];
int ply = 0;

struct Board
{ 
public:
    uint64_t* board = new uint64_t[12];
    uint64_t* white = board;  // represents white's bitboards (P,N,B,R,Q,K)
    uint64_t* black = board + 6;  // represents black's bitboards (p,n,b,r,q,k)
    uint8_t castles = 0b00000000;       // represents castling ability in 0b0000KQkq
    int8_t side = 1;                    // side to move, 1 for white, -1 for black
    int8_t enpassant = -1;              // enpassant square (-1 for none)
    uint8_t halfmoves;                  // halfmoves
    uint8_t fullmoves;                  // fullmoves
    Board()                             // Board Constructor
    {
        fullmoves = 0;
        halfmoves = 0;
        for (int i = 0; i < 12; i++)
        {
            *(board+i) = 0ULL;
        }
    }
    ~Board() {delete[] this->board, this;}
};

inline uint64_t whites(Board* bd) {return bd->white[0] | bd->white[1] | bd->white[2] | bd->white[3] | bd->white[4] | bd->white[5];}
inline uint64_t blacks(Board* bd) {return bd->black[0] | bd->black[1] | bd->black[2] | bd->black[3] | bd->black[4] | bd->black[5];}
inline uint64_t all(Board* bd) {return whites(bd) | blacks(bd);}

void import_FEN(Board* bd, std::string FEN)
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

std::string print_board(Board* bd)
{
    std::string boardstr = "--|---|---|---|---|---|---|--\n";
    char p[12] = {'P','N','B','R','Q','K','p','n','b','r','q','k'};
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

static inline void copy_from(Board* target, Board* origin)
{
    for (int i = 0; i < 12; i++)
    {
        target->board[i] = origin->board[i];
    }
    target->castles = origin->castles;
    target->side = origin->side;
    target->enpassant = origin->enpassant;
    target->halfmoves = origin->halfmoves;
    target->fullmoves = origin->fullmoves;
}

static inline void movef(Board* bd, uint32_t move)
{
    
    bd->halfmoves++;
    int end = end_square(move);
    int start = start_square(move);
    int moving_side = 0;
    bd->enpassant = -1;
    if ((1ULL << start) & bd->white[0])
    {
        if (end - start == -16) bd->enpassant = start - 8;
        bd->halfmoves = 0;
    }
    else if ((1ULL << start) & bd->black[0])
    {
        if (end - start == 16)  bd->enpassant = start + 8;
        bd->halfmoves = 0;
    }
    if (capture_flag(move)) bd->halfmoves = 0;
    for (int i = 0; i < 6; i++)
    {
        
        bd->white[i] &= ~(1ULL << end);
        bd->black[i] &= ~(1ULL << end);
        if (bd->white[i] & (1ULL << start)) 
        {
            bd->white[    ((promotion_piece(move) == 0UL) ? i : promotion_piece(move) )    ] |= (1ULL << end);
            moving_side = 1;
        }
        
        if (bd->black[i] & (1ULL << start)) 
        {
            bd->black[    ((promotion_piece(move) == 0UL) ? i : promotion_piece(move) )    ]  |= (1ULL << end);
        }
        bd->white[i] &= ~(1ULL << start);
        bd->black[i] &= ~(1ULL << start);
    }
    bd->side = -(bd->side);
    if (bd->side == 1) bd->fullmoves++;
    bd->castles &= castle_overrides(move);
    if (castle_flag(move)) 
    {
        if (moving_side)    
        {
            bd->white[3] = push_bit(bd->white[3], ((end + start) / 2));
            bd->white[3] = pop_bit(bd->white[3], ((end > start) ? _H1 : _A1));
        }
        else                
        {
            bd->black[3] = push_bit(bd->black[3], ((end + start) / 2));
            bd->black[3] = pop_bit(bd->black[3], ((end > start) ? _H8 : _A8));
        }
    }
    if (enpassant_flag(move)) 
    {
        bd->white[0] &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
        bd->black[0] &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
    }
}

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

static inline bool check_check(Board* bd, unsigned int move)
{
    Board* tst = new Board[1];
    memcpy(tst, bd, sizeof(*tst));
}

#endif
#ifndef ChessBoard
#define ChessBoard

struct Board;

#include <bits/stdc++.h>
#include <sstream>

#include "Tools.hh"

#define white(i) board[i]
#define black(i) board[i+6] 


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

static inline uint64_t whites(const Board& bd) {return bd.board[0] | bd.board[1] | bd.board[2] | bd.board[3] | bd.board[4]  | bd.board[5] ;}
static inline uint64_t blacks(const Board& bd) {return bd.board[6] | bd.board[7] | bd.board[8] | bd.board[9] | bd.board[10] | bd.board[11];}
static inline uint64_t all(const Board& bd) {return whites(bd) | blacks(bd);}

void import_FEN(Board* bd, const std::string FEN)
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

const static inline void copy_from(Board* target, const Board& origin)
{
    memcpy(target, &(origin), sizeof(Board));
        /*for (int i = 0; i < 12; i++)
        {
            target->board[i] = origin->board[i];
        }
    target->castles = origin.castles;
    target->side = origin.side;
    target->enpassant = origin.enpassant;
    target->halfmoves = origin.halfmoves;
    target->fullmoves = origin.fullmoves;*/
}

const static inline void movef(Board* bd, const moveWrapper& move)
{
    
    bd->halfmoves++;
    if (capture_flag(move)) bd->halfmoves = 0;
    const int end = end_square(move);
    const int start = start_square(move);
    const int moving_side = (bd->side == 1) ? 0 : 6;
    bd->side = -(bd->side);
    if (bd->side == 1) bd->fullmoves++;
    bd->castles &= castle_overrides(move);
    bd->enpassant = -1;
    if (moved_piece(move) == 0)
    {
        if (double_pawn_flag(move)) bd->enpassant = start - 8;
        bd->halfmoves = 0;
    }
    else if (moved_piece(move) == 0)
    {
        if (double_pawn_flag(move)) bd->enpassant = start + 8;
        bd->halfmoves = 0;
    }
    bd->board[moving_side + moved_piece(move)] &= ~(1ULL << start);
    if (promotion_piece(move))
    {
        bd->board[moving_side + promotion_piece(move)] |= 1ULL << end;
    }
    else
    {
        bd->board[moving_side + moved_piece(move)] |= 1ULL << end;
    }
    if (castle_flag(move)) 
    {
        if (moving_side)    
        {
            bd->white(3) = push_bit(bd->white(3), ((end + start) / 2));
            bd->white(3) = pop_bit(bd->white(3), ((end > start) ? _H1 : _A1));
        }
        else                
        {
            bd->black(3) = push_bit(bd->black(3), ((end + start) / 2));
            bd->black(3) = pop_bit(bd->black(3), ((end > start) ? _H8 : _A8));
        }
    }
    if (enpassant_flag(move)) 
    {
        bd->white(0) &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
        bd->black(0) &= ~(1ULL << (end + ((moving_side) ? 8 : -8)));
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


#endif
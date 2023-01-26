#ifndef ChessBoard
#define ChessBoard


#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include "Moves.hh"
#include "Eval.hh"
#include "Tools.hh"

class Board
{ 
public:
    uint64_t* white = new uint64_t[6];  // represents white's bitboards (P,N,B,R,Q,K)
    uint64_t* black = new uint64_t[6];  // represents black's bitboards (p,n,b,r,q,k)
    uint8_t castles = 0x00000000;       // represents castling ability in 0x0000KQkq
    int8_t side = 1;                    // side to move, 1 for white, -1 for black
    int8_t enpassant = -1;              // enpassant square (-1 for none)
    uint8_t halfmoves;                  // halfmoves
    uint8_t fullmoves;                  // fullmoves
    int8_t heuristic = 0;               // heuristic used for optimization
    Board()                             // Board Constructor
    {
        fullmoves = 0;
        halfmoves = 0;
        for (int i = 0; i < 6; i++)
        {
            *(white+i) = 0ULL;
            *(black+i) = 0ULL;
        }
    }
    ~Board()                            // Board Destructor
    {
        delete[] this->black, this->white;  // delete heap-allocated arrays
    }
    void import_FEN(std::string FEN);       // declaration for void Board::import_FEN(), takes a string parameter
    std::string to_string();                // declaration for std::string Board::to_string()
    
};

void Board::import_FEN(std::string FEN)
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
    for (int pos = 0; pos < 64; pos++)
    {
        uint64_t itr = pow(2,pos);
        switch (arr[pos])
        {
            case 'P': *(white + 0) |= itr; break;
            case 'N': *(white + 1) |= itr; break;
            case 'B': *(white + 2) |= itr; break;
            case 'R': *(white + 3) |= itr; break;
            case 'Q': *(white + 4) |= itr; break;
            case 'K': *(white + 5) |= itr; break;

            case 'p': *(black + 0) |= itr; break;
            case 'n': *(black + 1) |= itr; break;
            case 'b': *(black + 2) |= itr; break;
            case 'r': *(black + 3) |= itr; break;
            case 'q': *(black + 4) |= itr; break;
            case 'k': *(black + 5) |= itr; break;
        }
    }
    feni++;
    if (FEN[feni] == 'w') {side = 1;} else {side = -1;}
    feni += 2;
    for (feni; FEN[feni] != ' '; feni++)
    {
        switch (FEN[feni])
        {
            case 'K': castles |= 0x00001000; break;
            case 'Q': castles |= 0x00000100; break;
            case 'k': castles |= 0x00000010; break;
            case 'q': castles |= 0x00000001; break;
        }
    }
    feni++;
    if (FEN[feni] != '-')
    {
        enpassant = 0;
        enpassant += FEN[feni] - 'a';
        feni++;
        enpassant += 8 * (7 - (FEN[feni] - '1'));
    }
    else {enpassant = -1;}
    feni += 2;
}

std::string Board::to_string()
{
    std::string bd = "";
    char wp[6] = {'P','N','B','R','Q','K'};
    char bp[6] = {'p','n','b','r','q','k'};
    for (int pos = 0; pos < 64; pos++)
    {
        uint64_t itr = std::pow(2,pos);
        int piece;
        for (piece = 0; piece < 6; piece++)
        {
            if (*(black+piece) & itr) {bd += bp[piece]; break;}
            else if (*(white+piece) & itr) {bd += wp[piece]; break;}
        }
        if (piece != 6) {;}
        else if ((pos + (pos / 8)) % 2 == 1) {bd += '#';}
        else {bd += ' ';}
        if (pos % 8 == 7) {bd += '\n';}
    }
    return bd;
}


#endif
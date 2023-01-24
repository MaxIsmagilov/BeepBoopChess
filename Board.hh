#ifndef ChessBoard
#define ChessBoard
#define U64 unsigned long long
#include <bits/stdc++.h>


class Board
{
public:
    U64* white = new U64[6];
    U64* black = new U64[6];
    uint8_t booleans = 0x00000000;
    int8_t enpassant = -1;
    uint8_t halfmoves;
    uint8_t fullmoves;
    Board() 
    {
        fullmoves = 0;
        halfmoves = 0;
        for (int i = 0; i < 6; i++)
        {
            *(white+i) = 0ULL;
            *(black+i) = 0ULL;
        }
    }
    ~Board() 
    {
        delete[] this->black, this->white;
    }
    void import_FEN(std::string FEN);
};

void Board::import_FEN(std::string FEN)
{

}


#endif
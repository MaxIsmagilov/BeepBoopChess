#ifndef ChessBoard
#define ChessBoard
#define U64 unsigned long long
#include <bits/stdc++.h>
#include <string>
#include <sstream>

class Board
{
public:
    U64* white = new U64[6];
    U64* black = new U64[6];
    uint8_t castles = 0x00000000;
    int8_t side = 1;
    int8_t enpassant = -1;
    uint8_t halfmoves;
    uint8_t fullmoves;
    int8_t heuristic = 0;
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
    std::string to_string();
    
};

void Board::import_FEN(std::string FEN)
{
    char arr[64];  
    int index;
    int feni = 0;
    for (index = 0; FEN[index] != ' '; index++)
    {
        if (FEN[index] == '/') {continue;}
        else if (std::isdigit(FEN[index])) 
        {
            for (int i = 0; i <= (FEN[index] - '1'); i++)
            {
                arr[feni] = '\0';
                feni++;
            }
        }
        else 
        {
            arr[feni] = FEN[index];
            feni++;
        }
    }
    for (int pos = 0; pos < 64; pos++)
    {
        U64 itr = pow(2,pos);
        uint64_t* dfault;
        uint64_t** piece = &dfault;
        switch (arr[pos])
        {

        }
    }
}

std::string Board::to_string()
{
    std::string bd = "";
    char wp[6] = {'P','N','B','R','Q','K'};
    char bp[6] = {'p','n','b','r','q','k'};
    for (int pos = 0; pos < 64; pos++)
    {
        U64 itr = std::pow(2,pos);
        for (int piece = 0; piece < 6; piece++)
        {
            if (*(black+piece) & itr) {bd += bp[piece];}
            else if (*(white+piece) & itr) {bd += wp[piece];}
            
        }
        if ((pos + (pos / 8)) % 2 == 1) {bd += '@';}
        else {bd += ' ';}
        if (pos % 8 == 7) {bd += '\n';}
    }
    return bd;
}


#endif
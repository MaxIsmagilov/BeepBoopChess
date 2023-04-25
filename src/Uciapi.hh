#ifndef UCIAPI
#define UCIAPI

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "NegaMax.hh"
#include "Board.hh"
#include "Eval.hh"
#include "Random.hh"

Board MAIN_BOARD = Board();

move_wrapper pull_move(std::string mv, Board* bd)
{
    std::array<move_wrapper, 120> arr = std::array<move_wrapper, 120>();
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
    for (move_wrapper i : arr)
    {
        if (start_square(i) == source && end_square(i) == end && promotion == promotion_piece(i))
            return i;
    }
    return {0,0};
}

void print_move(const move_wrapper& move)
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
    int i = 1;
    printf("\n\t");
    std::array<move_wrapper, 120> arr = std::array<move_wrapper, 120>();
    get_moves(bd, arr.begin());
    for (move_wrapper mw : arr)
    {
        unsigned int j = mw._mv;
        printf("%i:", i++);
        print_move(mw);
        printf("\n\t");
    }
    printf("\n");
}

void uci_loop()
{
    while (false) 
    {
        
    }
}

#endif
#ifndef UCIAPI
#define UCIAPI

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "NegaMax.hh"
#include "Board.hh"
#include "Moves.hh"
#include "Eval.hh"
#include "Tools.hh"
#include "Random.hh"

unsigned int pull_move(std::string mv, Board* bd)
{
    std::vector<unsigned int> vec = std::vector<unsigned int>();
    vec.resize(120);
    get_moves(bd, bd->side, &vec);

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
    for (unsigned int i : vec)
    {
        if (start_square(i) == source && end_square(i) == end && promotion == promotion_piece(i))
            return i;
    }
    return 0;
}

#endif
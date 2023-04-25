#ifndef UCIAPI
#define UCIAPI

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <algorithm>

#include "NegaMax.hh"
#include "Board.hh"
#include "Eval.hh"
#include "Random.hh"

#define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

Board MAIN_BOARD = Board();

move_wrapper pull_move(std::string mv, Board* bd)
{
    std::array<move_wrapper, 120> arr = std::array<move_wrapper, 120>();
    get_moves(*bd, arr.begin());

    unsigned int source = (mv[0] - 'a') + (8 - (mv[1] - '0')) * 8;
    unsigned int end = (mv[2] - 'a') + (8 - (mv[3] - '0')) * 8;
    unsigned int promotion = 0U;
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
        piece = 'n'; break;
        case 2UL:
        piece = 'b'; break;
        case 3UL:
        piece = 'r'; break;
        case 4UL:
        piece = 'q'; break;
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

void parse_input(const std::string& input_string)
{
    if (input_string.substr(0,8) == "position")
    {
        if (input_string.substr(0,17) == "position startpos")
        {
            import_FEN(&MAIN_BOARD, START_POSITION);
            if (input_string.size() != 17)
            {
                int move_position = 23;
                do
                {
                    const int move_end = [&]() -> int
                    {
                        return std::find_if(&input_string[move_position], &input_string[move_position] + (input_string.size()- move_position), [&](const char& c) -> bool{return c == ' ';}) - &input_string[0];
                    }();
                    movef(MAIN_BOARD, pull_move(input_string.substr(move_position, move_end - move_position), &MAIN_BOARD));
                    move_position = move_end+1;
                } while (move_position < input_string.size());
            }
        }
        else if (input_string.substr(0,12) == "position fen")
        {
            bool has_moves;
            const int fen_end = [&]() -> int
            {
                return std::find_if(&input_string[12], &input_string[input_string.length()+1], [&](const char& c){has_moves = (c == 'm');return has_moves;}) - &input_string[1];
            }();
            import_FEN(&MAIN_BOARD, input_string.substr(13, fen_end));
            if (has_moves)
            {
                int move_position = fen_end + 7;
                do
                {
                    const int move_end = [&]() -> int
                    {
                        return std::find_if(&input_string[move_position], &input_string[move_position] + (input_string.size()- move_position), [&](const char& c) -> bool{return c == ' ';}) - &input_string[0];
                    }();
                    movef(MAIN_BOARD, pull_move(input_string.substr(move_position, move_end - move_position), &MAIN_BOARD));
                    move_position = move_end+1;
                } while (move_position < input_string.size());
            }
        }
    }

    else if (input_string.substr(0,2) == "go") //"go depth 6"
    {
        int depth = std::stoi(input_string.substr(9));
        printf("%i <- depth", depth);
    }
}

void uci_loop()
{
    while (false) 
    {
        
    }
}

#endif
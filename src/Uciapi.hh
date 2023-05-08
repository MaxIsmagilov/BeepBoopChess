/*
 * time controls forked from BBC by CMK who forked it from VICE by Richard Allbert
 *
 */



#ifndef UCIAPI
#define UCIAPI

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <algorithm>

#include "Board.hh"
#include "Eval.hh"
#include "Random.hh"

#define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

Board MAIN_BOARD;

static constexpr int BUFFER_SIZE = 120;



static inline move_wrapper pull_move(std::string mv, Board* bd)
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

static inline void print_move(const move_wrapper& move, const bool& new_line)
{
    char sourcef = 'a' + (start_square(move) % 8);
    char sourcer = '8' - (start_square(move) / 8);
    char endf = 'a' + (end_square(move) % 8);
    char endr = '8' - (end_square(move) / 8);
    
    char piece;
    switch (promotion_piece(move))
    {
        case 0UL: 
                        printf("%c%c%c%c%c", sourcef, sourcer, endf, endr, (new_line) ? '\n' : ' '); return;
        case 1UL:
        piece = 'n'; break;
        case 2UL:
        piece = 'b'; break;
        case 3UL:
        piece = 'r'; break;
        case 4UL:
        piece = 'q'; break;
    }
    printf("%c%c%c%c%c%c", sourcef, sourcer, endf, endr, piece, (new_line) ? '\n' : ' ');
}

static inline void print_moves(const Board& bd)
{
    int i = 1;
    printf("\n\t");
    std::array<move_wrapper, 120> arr = std::array<move_wrapper, 120>();
    get_moves(bd, arr.begin());
    for (move_wrapper mw : arr)
    {
        unsigned int j = mw._mv;
        printf("%i:", i++);
        print_move(mw, true);
        printf("\n\t");
    }
    printf("\n");
}

static inline int parse_input(const std::string& input_string)
{
    if (input_string.substr(0,8) == "position")
    {
        if (input_string.substr(0,17) == "position startpos")
        {
            import_FEN(&MAIN_BOARD, START_POSITION);
            if (input_string.size() != 17)
            {
                int move_position = 24;
                do
                {
                    const int move_end = [&]() -> int
                    {
                        return std::find_if(&input_string[move_position], &input_string[move_position] + (input_string.size()- move_position), [&](const char& c) -> bool{return c == ' ';}) - &input_string[0];
                    }();
                    MAIN_BOARD.make_move(pull_move(input_string.substr(move_position, move_end - move_position), &MAIN_BOARD));
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
                    MAIN_BOARD.make_move(pull_move(input_string.substr(move_position, move_end - move_position), &MAIN_BOARD));
                    move_position = move_end+1;
                } while (move_position < input_string.size());
            }
        }
    }

    else if (input_string.substr(0,2) == "go") //"go depth 6" "go binc 123124 winc 21301"
    {
        int depth = -1;
        depth = std::stoi(input_string.substr(9));
        char* value;

        if (value = strstr(input_string.c_str(), "infinity"));
        if ((value = strstr(input_string.c_str(), "binc")) && MAIN_BOARD.get_side_to_move() == -1)
            pub_time._inc = std::atoi(value + 5);
        if ((value = strstr(input_string.c_str(), "winc")) && MAIN_BOARD.get_side_to_move() == 1)
            pub_time._inc = std::atoi(value + 5);
        if ((value = strstr(input_string.c_str(),"wtime")) && MAIN_BOARD.get_side_to_move() == 1)
            pub_time._time = atoi(value + 6);
        if ((value = strstr(input_string.c_str(),"btime")) && MAIN_BOARD.get_side_to_move() == -1)
            pub_time._time = atoi(value + 6);
        if ((value = strstr(input_string.c_str(),"movestogo")))
            pub_time._movestogo = atoi(value + 10);
        if ((value = strstr(input_string.c_str(),"movetime")))
            pub_time._movetime = atoi(value + 9);
        if ((value = strstr(input_string.c_str(),"depth")))
            depth = atoi(value + 6);

        if(pub_time._movetime != -1)
        {
            pub_time._time = pub_time._movetime;
            pub_time._movestogo = 1;
        }

        pub_time._starttime = high_resolution_clock::now();;

        depth = depth;

        if(pub_time._time != -1)
        {
            pub_time._timeset = 1;

            pub_time._time /= pub_time._movestogo;
            
            if (pub_time._time > 1500) pub_time._time -= 50;
            
            pub_time._stoptime = pub_time._time + pub_time._inc;
            
            if (pub_time._time < 1500 && pub_time._inc && depth == 64) pub_time._stoptime = pub_time._inc - 50;
        }

        if(depth == -1)
            depth = 64;


        const move_info best_move = get_best_move(&MAIN_BOARD, depth);
        print_move(best_move.move, true);
        printf("%10i\n", best_move.total_nodes);
        MAIN_BOARD.make_move(best_move.move);
    }
    else if (input_string.substr(0,7) == "readyok") std::cout << "isready\n";
    else if (input_string.substr(0,10) == "ucinewgame") parse_input("position startpos");
    else if (input_string.substr(0,4) == "quit") {return 1;}
    else if (input_string.substr(0,3) == "uci") {std::cout  << "id name BobChess\n" 
                                                            << "id author BL0OOP\n"
                                                            << "uciok" << std::endl;}
    return 0;
}

static inline std::string trim(const char* word, int size)
{
    std::string result = "";
    std::find_if(word, word+size, [&](const char& c) mutable -> bool 
        {
            if (c == '\0') return true;
            result += c;
            return false;
        });
    return result;
}

static inline void uci_loop()
{
    
    std::cout << "id name BobChess\n" 
              << "id author BL0OOP\n"
              << "uciok" << std::endl;
    char input[BUFFER_SIZE];
    while (true) 
    {
        std::cin.sync();
        if (std::cin.getline(input, BUFFER_SIZE))
        {
            if(parse_input(trim(input, BUFFER_SIZE))) break;
        }
        std::cout << to_string(&MAIN_BOARD);
        //print_move_values(MAIN_BOARD);
    }
}

#endif
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "NegaMax.hh"
#include "Board.hh"
#include "Moves.hh"
#include "Eval.hh"
#include "Tools.hh"
#include "Random.hh"
#include "Uciapi.hh"

void setup()
{

    std::cout << "starting setup...\n";
    std::cout << "initializing non-sliders...\n";
    initialize_non_sliders();
    std::cout << "creating magic numbers...\n";
    // initialize_magic_numbers();
    std::cout << "initializing sliders...\n";
    initialize_sliders();
    std::cout << "initializing eval tables... \n";
    initialize_evaluation();
    std::cout << "setup complete\n\n";
}

void print_moves(Board* bd)
{
    std::vector<unsigned int>* vec = new std::vector<unsigned int>();
    get_moves(bd, vec);
    for (unsigned int j : *vec)
    {
        print_move(j);
        printf(", ");
    }
    printf("\n\t");
}

int main(int argc, char const *argv[])
{
    setup();
    Board start = Board();
    start.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //start.import_FEN("r1b2rk1/p1b1qppp/2pp4/4n3/NP1QP3/P4B2/1B3PPP/R4RK1 w - - 5 17");
    std::vector<unsigned int> moves = std::vector<unsigned int>();
    std::cout << start.to_string() << "\n";
    int depth;
    int plies = 1;
    std::cout << "enter depth: ";
    std::cin >> depth;
    int i = 0;
    while (i < plies)
    {
        unsigned int move;
        if (i%2 == 2)
        {
            std::string movestring;
            std::cout << "enter your move: ";
            std::cin >> movestring;
            move = pull_move(movestring, &start);
        }
        else
        {
            move = get_best_move(&start, depth);
            printf("\tused: ");
            print_move(move);
            printf("\n");
            
        }
        if (!move)
        {
            std::cout << "endpoint reached or illegal move\n";
            break;
        }
        start.move(move);
        std::cout << start.to_string() << "\n";
        i++;
    }
}

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "NegaMax.hh"
#include "Board.hh"
#include "Moves.hh"
#include "Eval.hh"
#include "Tools.hh"
#include "Random.hh"

void setup()
{
    
    std::cout << "starting setup...\n";
    std::cout << "initializing non-sliders...\n";
    initialize_non_sliders();
    std::cout << "creating magic numbers...\n";
    //initialize_magic_numbers();
    std::cout << "initializing sliders...\n";
    initialize_sliders();
    std::cout << "setup complete\n\n";
}

int main(int argc, char const *argv[])
{
    setup();
    Board start = Board();
    start.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::vector<unsigned int> moves = std::vector<unsigned int>();
    std::cout << start.print_board() << "\n";
    int depth;
    int plies;
    std::cout << "enter depth and plies:";
    std::cin >> depth >> plies;
    int i = 0;
    while (i < plies)
    {
        unsigned int move = get_best_move(&start, depth);
        if (!move) {std::cout << "endpoint reached\n"; break;}
        start.move((get_best_move(&start, depth)));
        std::cout << start.print_board() << "\n";
        i++;
    }
}



#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
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
    uint32_t move = pack_move(_E2, _E4, 0, 0, 0, 0, 0);
    //std::cout << move;
    start.move(move);
    std::cout << start.to_string() << is_attacked(_E1, start, 1);
}



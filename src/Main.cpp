#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <chrono>

#include "NegaMax.hh"
#include "Board.hh"
#include "Eval.hh"
#include "Random.hh"
#include "Uciapi.hh"

using namespace std::chrono;

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

int main(int argc, char const *argv[]) 
{
    setup();
    

    parse_input("position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5");
    std::cout << to_string(&MAIN_BOARD);

    return 0;

        /*const auto begin = high_resolution_clock::now();
        const move_info move = {{0,0},0,0};
        const auto end = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(end - begin);
        const float speed =  0.0F;*/

} 

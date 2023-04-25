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
    Board start = Board();
    import_FEN(&start,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //print_move_values(start);
    run_perft(start);

    return 0;
    //import_FEN(&start,"r1b2rk1/p1b1qppp/2pp4/4n3/NP1QP3/P4B2/1B3PPP/R4RK1 w - - 5 17");
    std::array<unsigned int, 120> arr = std::array<unsigned int, 120>();
    std::cout << to_string(&start) << "\n";
    int depth;
    int plies = 1;
    std::cout << "enter depth: ";
    std::cin >> depth;
    int i = 0;
    while (i < plies)
    {
        const auto begin = high_resolution_clock::now();
        const move_info move = get_best_move(&start, depth);
        const auto end = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(end - begin);
        const float speed =  ((float) move.total_nodes) / duration.count();

        printf("\n\tvalue: %+4.2f, %lli nodes evaluated @ %4.1fk node/s\n\t", move.eval/100.0 * start.side, move.total_nodes, speed);
        printf("\tused: ");
        print_move(move.move);
        printf("\n");

        if (!move.move._mv)
        {
            std::cout << "endpoint reached or illegal move\n";
            break;
        }
        movef(&start, move.move);
        std::cout << to_string(&start) << "\n";
        i++;
    } 
} 

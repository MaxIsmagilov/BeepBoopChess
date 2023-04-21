#ifndef NegaMaxRunner
#define NegaMaxRunner

#include <vector>
#include <bits/stdc++.h>
#include <array>
#include <chrono>


#include "Eval.hh"
#include "Moves.hh"
#include "Board.hh"
#include "Tools.hh"


using namespace std::chrono;


struct moveinfo 
{
    moveWrapper move;
    long long int total_nodes;
    float eval;
};

const unsigned int ARRAY_SIZE = 120;

static long long int total_nodes;


Board* history[256];
std::array<moveWrapper, ARRAY_SIZE> move_arrays[256];

static int ply = 0;

static inline int quescence_search(int alpha, int beta)
{
    return 0;
}

static inline int negamax(int depth, int alpha, int beta, int side)
{
    total_nodes++; 
    Board& bd = *history[ply];
    // return if done or draw
    if (depth == 0) {return -side * eval(bd);}
    if (bd.halfmoves >= 50) return 0;

    // create and check moves
    get_moves(bd, move_arrays[ply].begin());
    if (move_arrays[ply][0]._mv == 0) 
    {
        bool i_c = in_check(bd,side);
        if (i_c) return (100000 + depth);
        else return 0;
    }

    /*   start of NegaMax   */

    // set initial value
    int value = -1000000;
    ply++;
    for (moveWrapper i : move_arrays[ply-1])
    {
        // check for null results
        if (i._mv == 0) break;

        // late move pruning
       // if (!(capture_flag(i)) && depth == 1 && !(in_check(bd,side))) {ply--; return -side * eval(bd);}
        
        // move the check board
        copy_from(history[ply], *history[ply-1]);
        movef(history[ply], i);

        // recursively evaluate next node
        int check_value = -negamax(depth - 1, -beta, -alpha, -side);
        
        // set value as the most valuable node
        value = std::max(check_value,value);

        // set alpha
        alpha = std::max(alpha, value);

        // alpha-beta cutoff
        if (alpha >= beta) {value = alpha; break;}
    }
    ply--;
    

    // return the value of the node
    return value;
}


static moveinfo get_best_move(Board* bd, int depth)
{
    for (int i = 0; i < 256; i++) {history[i] = new Board(); move_arrays[i] = std::array<moveWrapper, ARRAY_SIZE>();}
    ply = 0;
    if (bd->halfmoves >= 50) return {{0,0}, 0LL, 0.0F};
    total_nodes = 0;

    std::array<moveWrapper, ARRAY_SIZE> arr = std::array<moveWrapper, ARRAY_SIZE>();

    get_moves(*bd, arr.begin());
    if (arr[0]._mv == 0) return {{0,0},0,0.0F};

    int max_index = 0;
    int best_move_value = -1000000;
    int progress = 0;
    int i;
    unsigned int arr_size;

    for (arr_size = 0; arr[arr_size]._mv; arr_size++);

    printf("|--------------------|\n ");
    for (i = 0; i < arr_size; i++)
    {
        ply = 0;
        if (!arr[i]._mv) break;
        copy_from(history[0], *bd);
        movef(history[0], arr[i]);
        int value = negamax(depth, -1000000, 1000000, history[0]->side);

        //printf("0x%lx, %+4.2f\n", vec[i] , ((float) value * bd->side/ 100.0));
        if (value > best_move_value)
        {
            max_index = i;
            best_move_value = value;
        }
        PROGRESS_CHECK:
        if (((i * 20)/arr_size) >= progress)
        {
            progress++;
            printf("*");
            goto PROGRESS_CHECK;
        }
    }
    return {arr[max_index] , total_nodes, ((float) best_move_value * bd->side/ 100.0F)};
}

/*
function negamax(node, depth, α, β, color) is
    if depth = 0 or node is a terminal node then
        return color × the heuristic value of node

    childNodes := generateMoves(node)
    childNodes := orderMoves(childNodes)
    value := −∞
    foreach child in childNodes do
        value := max(value, −negamax(child, depth − 1, −β, −α, −color))
        α := max(α, value)
        if α ≥ β then
            break (* cut-off *)
    return value
*/



static inline int get_perft(int depth)
{
    if (depth == 0) return 1;
    int sum = 0;
    int sz = get_moves(*history[ply], move_arrays[ply].begin());
    //print_moves(*history[ply]);
    ply++;
    for (moveWrapper mv : move_arrays[ply-1])
    {
        if (!mv._mv) break;
        copy_from(history[ply], *history[ply-1]);
        movef(history[ply],mv);
        int inc = get_perft(depth-1);
        if (ply == 1) {print_move(mv); printf("\t%i\n", inc);}
        sum += inc;
    }
    ply--;
    return sum;
}


void run_perft(const Board& bd)
{
    for (int i = 0; i < 256; i++) 
    {
        history[i] = new Board(); 
        move_arrays[i] = std::array<moveWrapper, ARRAY_SIZE>();
    }
    for (int i = 0; i < 6; i++)
    {
        int ply = 0;
        int perftval = 0;
        copy_from(history[0], bd);
        auto begin = high_resolution_clock::now();
        perftval = get_perft(i);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - begin);
        float speed =  ((float) perftval) / duration.count();

        printf("\n%i:\t%i nodes @%4.1fk nodes/second\n\n",i ,perftval, speed);
    }
}
#endif
#ifndef NegaMaxRunner
#define NegaMaxRunner

#include <vector>
#include <bits/stdc++.h>
#include <array>

#include "Eval.hh"
#include "Moves.hh"
#include "Board.hh"
#include "Tools.hh"

struct moveinfo 
{
    unsigned int move;
    long long int total_nodes;
    float eval;
};

const unsigned int ARRAY_SIZE = 120;

static long long int total_nodes;


Board* history[256];
std::array<unsigned int, ARRAY_SIZE> move_arrays[256];

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
    if (move_arrays[ply][0] == 0) 
    {
        bool i_c = in_check(bd,side);
        if (i_c) return (100000 + depth);
        else return 0;
    }

    /*   start of NegaMax   */

    // set initial value
    int value = -1000000;
    ply++;
    for (unsigned int i : move_arrays[ply-1])
    {
        // check for null results
        if (i == 0) break;
        
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
    for (int i = 0; i < 256; i++) {history[i] = new Board(); move_arrays[i] = std::array<unsigned int, ARRAY_SIZE>();}
    ply = 0;
    if (bd->halfmoves >= 50) return {0U,0,0.0F};
    total_nodes = 0;

    std::array<unsigned int, ARRAY_SIZE> arr = std::array<unsigned int, ARRAY_SIZE>();

    get_moves(*bd, arr.begin());
    if (arr[0] == 0) return {0U,0,0.0F};

    int max_index = 0;
    int best_move_value = -1000000;
    int progress = 0;
    int i;
    unsigned int arr_size;

    for (arr_size = 0; arr[arr_size]; arr_size++);

    printf("|--------------------|\n ");
    for (i = 0; i < arr_size; i++)
    {

        if (!arr[i]) break;
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
#endif
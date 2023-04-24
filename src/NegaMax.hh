#ifndef NegaMaxRunner
#define NegaMaxRunner

#include <bits/stdc++.h>
#include <array>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <thread>
#include <mutex>

#include "Eval.hh"
#include "Moves.hh"
#include "Board.hh"
#include "Tools.hh"


using namespace std::chrono;

const int NUM_THREADS = 1;

struct moveinfo 
{
    moveWrapper move;
    long long int total_nodes;
    float eval;
    inline bool operator>(const moveinfo& other) const
    {
        return this->eval > other.eval;
    }
};

const int MAX_DEPTH = 256;

inline int quescence_search(int alpha, int beta)
{
    return 0;
}

static inline int negamax(int& ply, const int depth, int alpha, int beta, const int side, long long int& nodes, Board** current_bd, std::array<moveWrapper, ARRAY_SIZE>* current_arr)
{
    nodes++; 
    Board& bd = *current_bd[ply];
    // return if done or draw
    if (depth == 0) {return -side * eval(bd.board[0]);}
    if (bd.halfmoves >= 50) return 0;

    // create and check moves
    get_moves(bd, current_arr[ply].begin());
    if (current_arr[ply][0]._mv == 0) 
    {
        if (in_check(bd,-side)) return (100000 + depth);
        else return 0;
    }

    /*   start of NegaMax   */

    // set initial value
    ply++;
    int value = -1000000;
    std::find_if(current_arr[ply-1].begin(), current_arr[ply-1].begin() + ARRAY_SIZE, [&](moveWrapper i) mutable -> bool 
        {
        // check for null results
        if (i._mv == 0) return true;
        
        // move the check board
        copy_from(current_bd[ply], bd);
        movef(current_bd[ply], i);

        // recursively evaluate next node
        // set value as the most valuable node
        value = std::max(-negamax(ply, depth - 1, -beta, -alpha, -side, nodes, current_bd, current_arr),value);

        // set alpha
        alpha = std::max(alpha, value);

        // alpha-beta cutoff
        if (alpha >= beta) {value = alpha; return true;}
        return false;
        });
    // return the value of the node
    ply--;
    return value;
}

static inline moveinfo value_of(const int depth, const moveWrapper move, const Board& bd)
{
    Board* history[MAX_DEPTH];
    std::for_each(history, history+MAX_DEPTH, [&](Board*& bd) mutable {bd = new Board();});
    std::array<moveWrapper, ARRAY_SIZE> move_arrays[MAX_DEPTH];
    std::for_each(move_arrays, move_arrays+MAX_DEPTH, [&](std::array<moveWrapper, ARRAY_SIZE>& arr){arr = std::array<moveWrapper, ARRAY_SIZE>();});
    long long int total_nodes = 0;
    int ply = 0;
    copy_from(history[0], bd);
    float v = negamax(ply, depth, -1000000, 1000000, bd.side, total_nodes, history, move_arrays);
    return {move, total_nodes, v};
}

std::mutex array_lock;

static inline void run_threads(const int depth, const moveWrapper& mv, const Board& bd, std::vector<moveinfo>& eval_arr) 
{
    moveinfo mvnfo = value_of(depth, mv, bd);
    array_lock.lock();
    eval_arr.push_back(mvnfo);
    array_lock.unlock();
}

moveinfo get_best_move(const Board* bd, int depth)
{
    if (bd->halfmoves >= 50) return {{0,0}, 0LL, 0.0F};
    long long int total_nodes = 0;

    std::array<moveWrapper, ARRAY_SIZE> arr = std::array<moveWrapper, ARRAY_SIZE>();
    std::vector<moveinfo> eval_arrays = std::vector<moveinfo>();
    std::array<std::thread, NUM_THREADS> threads = std::array<std::thread, NUM_THREADS>();
    int current_thread = 0;

    long long int _total_nodes = 0;
    const int size = ARRAY_SIZE;

    get_moves(*bd, arr.begin());

    if (arr[0]._mv == 0) return {{0,0},0,0.0F};
    
    const int last_move = ([&]() -> int
    {
        int pos = 0;
        while (arr[pos]._mv)
        {
            threads.at(current_thread) = std::thread(run_threads, depth, arr[pos]._mv, *bd, eval_arrays);
            threads.at(current_thread).join();
            delete &threads.at(current_thread);
            current_thread = (current_thread == NUM_THREADS) ? 0 : current_thread + 1;
        }
        return pos;
    })();
    
    
    /*std::find_if((&arr[0]), (&arr[0])+size, [&](const moveWrapper& mv) -> bool
    {
        if (!mv._mv) return true;
        eval_arrays.push_back(value_of(depth, mv, *bd));
        return false;
    });*/


    std::for_each(eval_arrays.begin(), eval_arrays.begin()+last_move, [&](moveinfo mi) {_total_nodes += mi.total_nodes;});
    std::sort(eval_arrays.begin(), eval_arrays.begin()+last_move, std::greater<>());
    eval_arrays[0].total_nodes = _total_nodes;
    return {eval_arrays[0]};
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



static inline int get_perft(int depth, int& ply ,Board** history, std::array<moveWrapper, ARRAY_SIZE>* move_arrays)
{
    if (depth == 0) return 1;
    int sum = 0;
    const int sz = get_moves(*history[ply], move_arrays[ply].begin());
    //print_moves(*history[ply]);
    ply++;
    std::find_if(&move_arrays[ply-1][0], (&move_arrays[ply-1][0]) + ARRAY_SIZE, [&](moveWrapper mw) mutable -> bool
    {
        if (!mw._mv) return true;
        copy_from(history[ply], *history[ply-1]);
        movef(history[ply],mw);
        const int inc = get_perft(depth-1, ply, history, move_arrays);
        if (ply == 1) {print_move(mw); printf("\t%i\n", inc);}
        sum += inc;
        return false;
    });
    ply--;
    return sum;
}


void run_perft(const Board& bd)
{
    Board* history[MAX_DEPTH];
    std::for_each(history, history+MAX_DEPTH, [&](Board*& bd) mutable {bd = new Board();});
    std::array<moveWrapper, ARRAY_SIZE> move_arrays[MAX_DEPTH];
    std::for_each(move_arrays, move_arrays+MAX_DEPTH, [&](std::array<moveWrapper, ARRAY_SIZE>& arr){arr = std::array<moveWrapper, ARRAY_SIZE>();});
    long long int total_nodes = 0;

    for (int i = 0; i < 7; i++)
    {
        
        int ply = 0;
        copy_from(history[0], bd);
        copy_from(history[0], bd);
        const auto begin = high_resolution_clock::now();
        const int perftval = get_perft(i, ply, history, move_arrays);
        const auto end = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(end - begin);
        const float speed =  ((float) perftval) / duration.count();

        printf("\n%i:\t%i nodes @%4.1fk nodes/second\n\n",i ,perftval, speed);
    }
}
#endif
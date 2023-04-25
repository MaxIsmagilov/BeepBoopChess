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
#include "Board.hh"


using namespace std::chrono;

constexpr int NUM_THREADS = 1;
constexpr bool MULTITHREADING_ENABLED = false;  // do not enable multithreading it deletes the home directory (not really but it won't work)
constexpr int MAX_DEPTH = 256;

std::mutex array_lock;

void print_move(const move_wrapper& move);

struct move_info 
{
    move_wrapper move;
    long long int total_nodes;
    int eval;
    inline bool operator>(const move_info& other) const
    {
        return this->eval > other.eval;
    }
};

void print_move_info(const move_info& mvif)
{
    printf("move_info: "); print_move(mvif.move); printf("\tafter %10lli nodes -> %+4.1f\n", mvif.total_nodes, (float) mvif.eval/100.0);
}

inline int quescence_search(int alpha, int beta)
{
    return 0;
}

static inline int negamax(int& ply, const int depth, int alpha, int beta, const int side, long long int& nodes, Board** current_bd, std::array<move_wrapper, ARRAY_SIZE>* current_arr)
{
    nodes++; 
    Board& bd = *current_bd[ply];
    // return if done or draw
    if (depth == 0) {return side * eval(bd.board);}
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
    std::find_if(current_arr[ply-1].begin(), current_arr[ply-1].end(), [&](move_wrapper i) mutable -> bool 
        {
        // check for null results
        if (i._mv == 0) return true;
        
        // move the check board
        copy_from(current_bd[ply], *current_bd[ply-1]);
        movef(current_bd[ply], i);

        // recursively evaluate next node
        // set value as the most valuable node
        value = std::max(-negamax(ply, depth - 1, -beta, -alpha, -side, nodes, current_bd, current_arr), value);

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

static inline move_info value_of(const int depth, const move_wrapper move, const Board& bd)
{
    Board* history[MAX_DEPTH];
    std::for_each(history, history+MAX_DEPTH, [&](Board*& bd) mutable {bd = new Board();});
    std::array<move_wrapper, ARRAY_SIZE> move_arrays[MAX_DEPTH];
    std::for_each(move_arrays, move_arrays+MAX_DEPTH, [&](std::array<move_wrapper, ARRAY_SIZE>& arr){arr = std::array<move_wrapper, ARRAY_SIZE>();});
    long long int total_nodes = 0;
    int ply = 0;
    copy_from(history[0], bd);
    movef(history[0], move);
    int v = negamax(ply, depth, -1000000, 1000000, bd.side, total_nodes, &history[0], move_arrays);
    //printf("%i\n", (int)v);
    return {move, total_nodes, v};
}

static inline void run_threads(const int depth, const move_wrapper& mv, const Board& bd, std::vector<move_info>& eval_arr) 
{
    move_info mvnfo = value_of(depth, mv, bd);
    array_lock.lock();
    eval_arr.push_back(mvnfo);
    array_lock.unlock();
}

static inline move_info get_best_move(const Board* bd, int depth)
{
    if (bd->halfmoves >= 50) return {{0,0}, 0LL, 0};
    long long int total_nodes = 0;

    std::array<move_wrapper, ARRAY_SIZE> arr = std::array<move_wrapper, ARRAY_SIZE>();
    std::vector<move_info> eval_arrays = std::vector<move_info>();
    std::array<std::thread*, NUM_THREADS> threads = std::array<std::thread*, NUM_THREADS>();

    long long int _total_nodes = 0;
    const int size = ARRAY_SIZE;

    get_moves(*bd, arr.begin());

    if (arr[0]._mv == 0) return {{0,0},0,0};
    
    const int last_move = ([&]() -> int
    {
        if (MULTITHREADING_ENABLED)
        {
            int pos = 0;
            while (arr[pos]._mv)
            {
                std::for_each(threads.begin(), threads.end(), [&](std::thread* thread) mutable {if (!arr[pos]._mv) return; /*thread = new std::thread(run_threads, depth, arr[pos]._mv, *bd, eval_arrays)*/; pos++;});
                std::for_each(threads.begin(), threads.end(), [&](std::thread*thread) mutable {thread->join(); delete thread;});
            }
            return pos;
        }
        else 
        {
            const auto pos = std::find_if(arr.begin(), arr.end(), [&](const move_wrapper& mv) -> bool
            {
                if (!mv._mv) return true;
                eval_arrays.push_back(value_of(depth, mv, *bd));
                return false;
            });

            return pos - &arr[0];
        }
    })();

    std::for_each(eval_arrays.begin(), eval_arrays.begin()+last_move, [&](move_info mi) {_total_nodes += mi.total_nodes;});
    std::sort(eval_arrays.begin(), eval_arrays.begin()+last_move, std::greater<>());

    eval_arrays[0].total_nodes = _total_nodes;
    return {eval_arrays[0]};
}

static inline int get_perft(int depth, int& ply, Board** history, std::array<move_wrapper, ARRAY_SIZE>* move_arrays)
{
    if (depth == 0) return 1;
    int sum = 0;
    const int sz = get_moves(*history[ply], move_arrays[ply].begin());
    //print_moves(*history[ply]);
    ply++;
    std::find_if(&move_arrays[ply-1][0], (&move_arrays[ply-1][0]) + ARRAY_SIZE, [&](move_wrapper mw) mutable -> bool
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
    std::array<move_wrapper, ARRAY_SIZE> move_arrays[MAX_DEPTH];
    std::for_each(move_arrays, move_arrays+MAX_DEPTH, [&](std::array<move_wrapper, ARRAY_SIZE>& arr){arr = std::array<move_wrapper, ARRAY_SIZE>();});
    long long int total_nodes = 0;

    for (int i = 0; i < 7; i++)
    {
        
        int ply = 0;
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
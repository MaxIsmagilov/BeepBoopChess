#ifndef NegaMaxRunner
#define NegaMaxRunner

#include <vector>
#include <bits/stdc++.h>

#include "Eval.hh"
#include "Moves.hh"
#include "Board.hh"
#include "Tools.hh"

static int total_nodes;

static inline int quescence_search(int alpha, int beta)
{
    return 0;
}

static inline int negamax(Board* bd, int depth, int alpha, int beta, int side)
{
    total_nodes++;
    if (depth == 0) { return eval(bd);}
    if (bd->halfmoves >= 50) return 0;

    std::vector<unsigned int> vec = std::vector<unsigned int>();
    get_moves(bd, side, &vec);
    if (vec.size() == 0) 
    {
        if (in_check(bd,side)) return (100000 + depth);
        else return 0;
    }
    int value = -1000000;
    Board* check = new Board();
    for (unsigned int i : vec)
    {
        check->copy_from(bd);
        check->move(i);
        value = std::max(value, -negamax(check, depth - 1, -beta, -alpha, -side));
        alpha = std::max(alpha, value);
        if (alpha >= beta) break;
    }
    check->~Board();
    return value;
}

static inline unsigned int get_best_move(Board* bd, int depth)
{
    total_nodes = 0;
    std::vector<unsigned int> vec = std::vector<unsigned int>();
    vec.resize(120);
    get_moves(bd, bd->side, &vec);
    if (vec.size() == 0) {return 0;}
    int max_index = 0;
    int best_move_value = -1000000;
    for (int i = 0; i < vec.size(); i++)
    {
        if (!vec[i]) continue;
        Board pr = Board();
        pr.copy_from(bd);
        pr.move(vec[i]);
        int value = negamax(&pr, depth, -1000000, 1000000, bd->side);
        //printf("0x%lx, %+4.2f\n", vec[i] , ((float) value * bd->side/ 100.0));
        if (value > best_move_value)
        {
            max_index = i;
            best_move_value = value;
        }
    }
    printf("\tused: 0x%lx, value: %+4.2f, %i nodes evaluated\n", vec[max_index] , ((float) best_move_value * bd->side/ 100.0), total_nodes);
    return vec[max_index];
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
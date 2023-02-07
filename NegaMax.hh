#ifndef NegaMaxRunner
#define NegaMaxRunner

#include <vector>
#include <bits/stdc++.h>

#include "Eval.hh"
#include "Moves.hh"
#include "Board.hh"
#include "Tools.hh"


int negamax(Board bd, int depth, int alpha, int beta, int side)
{
    if (depth == 0) {return eval(bd) * bd.side;}
    std::vector<unsigned int> vec = std::vector<unsigned int>();
    get_moves(bd, side, &vec);
    if (vec.size() == 0) 
    {
        if (in_check(bd,side)) return 1000000;
        else return 0;
    }
    int value = -1000000;
    for (unsigned int i : vec)
    {
        Board check = Board();
        check.copy_from(bd);
        value = std::max(value, -negamax(check, depth -1, -beta, -alpha, -side));
        alpha = std::max(alpha, value);
        check.close();
        if (alpha >= beta) return beta-1;;
    }
    return value;
}

unsigned int get_best_move(Board bd, int depth)
{
    std::vector<unsigned int> vec = std::vector<unsigned int>();
    vec.resize(120);
    get_moves(bd, bd.side, &vec);
    if (vec.size() == 0) {return 0;}
    int max_index = 0;
    int best_move_value = -1000000;
    for (int i = 0; i < vec.size(); i++)
    {
        if (!vec[i]) continue;
        Board pr = Board();
        pr.copy_from(bd);
        pr.move(vec[i]);
        int value = negamax(pr, depth, -1000000, 1000000, 1);
        //printf("0x%lx, %i\n", vec[i] , value);
        if (value > best_move_value)
        {
            max_index = i;
            best_move_value = value;
        }
        pr.close();
    }
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
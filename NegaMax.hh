#ifndef NegaMaxRunner
#define NegaMaxRunner

#include <vector>
#include <bits/stdc++.h>

#include "Eval.hh"
#include "Moves.hh"
#include "Board.hh"
#include "Tools.hh"

static long long int total_nodes;

static inline int quescence_search(int alpha, int beta)
{
    return 0;
}

static inline int negamax(Board* bd, int depth, int alpha, int beta, int side)
{
    // increment total nodes visited
    total_nodes++;

    // return if done or draw
    if (depth == 0) {return side * -eval(bd);}
    if (bd->halfmoves >= 50) return 0;

    // create and check moves
    std::vector<unsigned int>* vec = new std::vector<unsigned int>();
    get_moves(bd, vec);
    if (vec->size() == 0) 
    {
        if (in_check(bd,side)) return (100000 + depth);
        else return 0;
    }

    /*   start of NegaMax   */

    // set initial value
    int value = -1000000;

    // create new board
    Board* check = new Board();

    for (unsigned int i : *vec)
    {
        // move the check board
        check->copy_from(bd);
        check->move(i);

        // recursively evaluate next node
        int check_value = -negamax(check, depth - 1, -beta, -alpha, -side);

        // set value as the most valuable node
        value = std::max(check_value,value);

        // set alpha
        alpha = std::max(alpha, value);

        // alpha-beta cutoff
        if (alpha >= beta) {value = alpha; break;}
    }
    
    // delete 
    vec->~vector();
    check->~Board();

    // return the value of the node
    return value;
}

static inline unsigned int get_best_move(Board* bd, int depth)
{
    if (bd->halfmoves >= 50) return 0;
    total_nodes = 0;
    std::vector<unsigned int> vec = std::vector<unsigned int>();
    vec.resize(120);
    get_moves(bd, &vec);
    if (vec.size() == 0) {return 0;}
    int max_index = 0;
    int best_move_value = -1000000;
    int progress = 0;
    int i;
    printf("|--------------------|\n ");
    for (i = 0; i < vec.size(); i++)
    {
        if (!vec[i]) continue;
        Board* pr = new Board();
        pr->copy_from(bd);
        pr->move(vec[i]);
        int value = negamax(pr, depth, -1000000, 1000000, pr->side);
        //printf("0x%lx, %+4.2f\n", vec[i] , ((float) value * bd->side/ 100.0));
        if (value > best_move_value)
        {
            max_index = i;
            best_move_value = value;
        }
        PROGRESS_CHECK:
        if (((i * 20)/vec.size()) >= progress)
        {
            progress++;
            printf("*");
            goto PROGRESS_CHECK;
        }
        pr->~Board();
    }
    printf("\n\tvalue: %+4.2f, %lli nodes evaluated\n\t", ((float) best_move_value * bd->side/ 100.0), total_nodes);
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
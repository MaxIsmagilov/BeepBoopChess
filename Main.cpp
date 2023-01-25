#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "Board.hh"

int main(int argc, char const *argv[])
{
    Board start = Board();
    start.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::cout << start.to_string();
}

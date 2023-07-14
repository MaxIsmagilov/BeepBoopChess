#include <iostream>
#include <string>

#include "move_generator.hpp"
#include "move_list.hpp"

using namespace BobChess;

int main() {
  MoveGenerator::initialize_all();
  std::cout << "start\n";
  Board bd;
  bd.import_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  std::cout << bd.debug_print() << '\n';
  MoveList m = MoveGenerator::generate_all(bd);
  for (int i = 0; i < m.get_size(); ++i) {
    std::cout << m[i].to_string() << '\n';
  }
  return 0;
}

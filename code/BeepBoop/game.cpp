#include "game.hpp"

#include <iostream>

#include "move_finder.hpp"

namespace BeepBoop
{

Game::Game() : m_table() {}
std::tuple<Move, std::size_t> Game::go_time(double time, std::size_t thread_count) {
  auto t = MoveFinder::get_best_move_time(m_board, time, Evaluator::eval, m_table, thread_count);
  m_board.make_move(std::get<0>(t));
  return t;
}

std::tuple<Move, std::size_t> Game::go_depth(int depth, std::size_t thread_count) {
  auto t = MoveFinder::get_best_move(m_board, depth, Evaluator::eval, m_table, thread_count);
  m_board.make_move(std::get<0>(t));
  return t;
}

void Game::set_pos(const Board& board) { m_board = board; }

void Game::set_pos(std::string FEN) { m_board.import_FEN(FEN.c_str()); }
void Game::set_pos() {
  set_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  std::cout << "d\n";
}
void Game::print() { std::cout << m_board.nice_print(); }

}  // namespace BeepBoop
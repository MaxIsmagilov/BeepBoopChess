
#include <thread>

#include "board.hpp"
#include "transposition.hpp"

namespace BeepBoop
{

class Game
{
 private:
  TTable m_table;
  Board m_board;

 public:
  Game();
  std::tuple<Move, std::size_t> go_time(double time, std::size_t thread_count = std::thread::hardware_concurrency());
  std::tuple<Move, std::size_t> go_depth(int depth, std::size_t thread_count = std::thread::hardware_concurrency());
  void set_pos(const Board& board);
  void set_pos(std::string FEN);
  void set_pos();
  void print();
};

}  // namespace BeepBoop
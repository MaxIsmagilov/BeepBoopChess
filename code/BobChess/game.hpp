
#include "board.hpp"
#include "transposition.hpp"

namespace BobChess
{

class Game
{
 private:
  TTable m_table;
  Board m_board;

 public:
  Game();
  std::tuple<Move, std::size_t> go_time(double time);
  std::tuple<Move, std::size_t> go_depth(int depth);
  void set_pos(const Board& board);
  void set_pos(std::string FEN);
  void set_pos();
  void print();
};

}  // namespace BobChess
#include "move_finder.hpp"

#include <any>
#include <future>
#include <iostream>

#include "move_generator.hpp"
#include "thread_pool.hpp"
#include "timekeeping.hpp"
#include "transposition.hpp"

namespace BobChess
{

std::tuple<Move, int> MoveFinder::get_best_move(const Board& bd, int depth, std::function<int(Board)> eval) {
  Clock c;
  ThreadPool tp;
  MoveList ml = MoveGenerator::generate_all(bd);
  std::vector<Algorithm> algoes;
  for (int As = 0; As < ml.get_size(); ++As) {
    algoes.emplace_back(Algorithm(bd.move_copy(ml[As]), eval));
  }

  int nodecount{0};

  for (int i = 1; i < depth; ++i) {
    tp.go(1);
    nodecount = 0;
    std::vector<std::future<std::any>> results;
    results.reserve(ml.get_size());
    for (int j = 0; j < ml.get_size(); ++j) {
      results[j] = tp.queue(&Algorithm::evaluate_move, &(algoes[j]), i);
    }
    for (int j = 0; j < ml.get_size(); ++j) {
      auto tupl = std::any_cast<std::tuple<int, int>>(results[j].get());
      ml.set_heuristic(j, std::get<0>(tupl));
      nodecount += std::get<1>(tupl);
    }
    tp.cancel_pending();
  }

  ml.sort();

  return std::make_tuple(ml[0], nodecount);
}

std::tuple<Move, int> MoveFinder::get_best_move_time(const Board& bd, double milliseconds,
                                                     std::function<int(Board)> eval) {
  Clock c;
  ThreadPool tp;
  MoveList ml = MoveGenerator::generate_all(bd);
  std::vector<Algorithm> algoes;
  for (int As = 0; As < ml.get_size(); ++As) {
    algoes.emplace_back(Algorithm(bd.move_copy(ml[As]), eval));
  }

  int nodecount{0};

  for (int i = 1; i <= 64 && !c.time_up(milliseconds); ++i) {
    nodecount = 0;
    std::vector<std::future<std::any>> results;
    results.reserve(ml.get_size());
    for (int j = 0; j < ml.get_size(); ++j) {
      results[j] = tp.queue(&Algorithm::evaluate_move, &(algoes[j]), i);
    }
    tp.go(1);
    for (int j = 0; j < ml.get_size(); ++j) {
      auto tupl = std::any_cast<std::tuple<int, int>>(results[j].get());
      ml.set_heuristic(j, std::get<0>(tupl));
      nodecount += std::get<1>(tupl);
    }
  }

  ml.sort();

  return std::make_tuple(ml[0], nodecount);
}
}  // namespace BobChess
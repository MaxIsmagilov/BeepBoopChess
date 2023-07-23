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

std::tuple<Move, std::size_t> MoveFinder::get_best_move(const Board& bd, int depth, std::function<int(Board)> eval) {
  ThreadPool tp;
  MoveList ml = MoveGenerator::generate_all(bd);
  std::vector<Algorithm> algoes;
  for (int As = 0; As < ml.get_size(); ++As) {
    algoes.emplace_back(Algorithm(bd.move_copy(ml[As]), eval));
  }

  std::size_t nodecount{0};
  tp.go(15);
  bool mate_threat = false;

  for (int i = 1; i < depth && !mate_threat; ++i) {
    nodecount = 0;
    std::vector<std::future<std::any>> results;
    results.resize(ml.get_size());
    for (int j = 0; j < ml.get_size(); ++j) {
      results[j] = tp.queue(&Algorithm::evaluate_move, &(algoes[j]), i);
    }
    for (int j = 0; j < ml.get_size(); ++j) {
      auto tupl = std::any_cast<std::tuple<int, std::size_t>>(results[j].get());
      ml.set_heuristic(j, std::get<0>(tupl));
      nodecount += std::get<1>(tupl);
      if (ml[i].get_heuristic() > 9900000) mate_threat = true;
    }
    tp.cancel_pending();
  }
  tp.finish();

  ml.sort();

  for (int i = 0; i < ml.get_size(); ++i) {
    std::cout << '\t' << ml[i].to_string() << '\t' << ml[i].get_heuristic() / (100.0) << '\n';
  }
  return std::make_tuple(ml[0], nodecount);
}

std::tuple<Move, std::size_t> MoveFinder::get_best_move_time(const Board& bd, double milliseconds,
                                                             std::function<int(Board)> eval) {
  Clock c;
  c.start();
  ThreadPool tp;
  MoveList ml = MoveGenerator::generate_all(bd);
  std::vector<Algorithm> algoes;
  for (int As = 0; As < ml.get_size(); ++As) {
    algoes.emplace_back(Algorithm(bd.move_copy(ml[As]), eval));
  }

  std::size_t nodecount{0};
  tp.go(15);
  bool mate_threat = false;

  for (int i = 1; i <= 64 && !c.time_up(milliseconds) && !mate_threat; ++i) {
    bool mate_threat = false;
    nodecount = 0;
    std::vector<std::future<std::any>> results;
    results.resize(ml.get_size());
    for (int j = 0; j < ml.get_size(); ++j) {
      results[j] = tp.queue(&Algorithm::evaluate_move, &(algoes[j]), i);
    }
    for (int j = 0; j < ml.get_size(); ++j) {
      auto tupl = std::any_cast<std::tuple<int, std::size_t>>(results[j].get());
      ml.set_heuristic(j, std::get<0>(tupl));
      nodecount += std::get<1>(tupl);
      if (ml[i].get_heuristic() > 9900000) mate_threat = true;
    }
    tp.cancel_pending();
  }
  c.stop();
  tp.finish();

  ml.sort();
  for (int i = 0; i < ml.get_size(); ++i) {
    std::cout << '\t' << ml[i].to_string() << '\t' << ml[i].get_heuristic() / (100.0) << '\n';
  }

  return std::make_tuple(ml[0], nodecount);
}
}  // namespace BobChess
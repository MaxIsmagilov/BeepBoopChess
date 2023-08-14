#include "move_finder.hpp"

#include <any>
#include <future>
#include <iostream>

#include "move_generator.hpp"
#include "thread_pool.hpp"
#include "timekeeping.hpp"
#include "transposition.hpp"

namespace BeepBoop
{

std::tuple<Move, std::size_t> MoveFinder::get_best_move(const Board& bd, int depth, std::function<int(Board)> eval,
                                                        TTable& table) {
  //

  // declare and start a clock
  Clock c;
  c.start();

  // create a threadpool and start it with 15 threads
  ThreadPool tp;
  tp.go(15);

  // generate moves
  MoveList ml = MoveGenerator::generate_all(bd);

  // check if movelist is empty
  if (!ml.get_size()) return std::make_tuple(Move{}, 0);

  // make an Algo vector
  std::vector<Algorithm> algoes;

  // add agloes to the vector
  for (int current_loc = 0; current_loc < ml.get_size(); ++current_loc) {
    // emplace an algorithm with the targeted eval and moved-copied board
    algoes.emplace_back(Algorithm(bd.move_copy(ml[current_loc]), eval, table));
  }

  std::size_t nodecount{0};
  bool mate_threat = false;

  for (int i = 0; i < depth && !mate_threat; ++i) {
    // inside a depth, call MTD(f)

    nodecount = 0;
    std::vector<std::future<std::any>> results;
    results.resize(ml.get_size());

    for (int j = 0; j < ml.get_size(); ++j) {
      results[j] = tp.queue(&Algorithm::evaluate_move, &(algoes[j]), i);
    }

    for (int j = 0; j < ml.get_size(); ++j) {
      auto tupl = std::any_cast<std::tuple<int, std::size_t>>(results[j].get());

      auto score = -std::get<0>(tupl);
      auto result = std::get<1>(tupl);

      nodecount += result;

      ml.set_heuristic(j, score);
    }
    tp.cancel_pending();
    std::cout << i + 1 << '\t' << c.elapsed_ms() / 1000.0 << '\t' << nodecount << '\n';
  }
  tp.finish();

  ml.sort();

  for (int i = 0; i < ml.get_size(); ++i) {
    std::cout << '\t' << ml[i].to_string() << '\t' << ml[i].get_heuristic() / (100.0) << '\n';
  }
  return std::make_tuple(ml[0], nodecount);
}

std::tuple<Move, std::size_t> MoveFinder::get_best_move_time(const Board& bd, double milliseconds,
                                                             std::function<int(Board)> eval, TTable& table) {
  Clock c;
  c.start();
  ThreadPool tp;
  MoveList ml = MoveGenerator::generate_all(bd);

  // check if movelist is empty
  if (!ml.get_size()) return std::make_tuple(Move{}, 0);

  std::vector<Algorithm> algoes;

  for (int current_loc = 0; current_loc < ml.get_size(); ++current_loc) {
    algoes.emplace_back(Algorithm(bd.move_copy(ml[current_loc]), eval, table));
  }

  std::size_t nodecount{0};
  tp.go(15);
  bool mate_threat = false;

  for (int i = 0; i <= 64 && !mate_threat && !c.time_up(milliseconds); ++i) {
    nodecount = 0;
    std::vector<std::future<std::any>> results;
    results.resize(ml.get_size());
    for (int j = 0; j < ml.get_size(); ++j) {
      results[j] = tp.queue(&Algorithm::evaluate_move, &(algoes[j]), i);
    }
    for (int j = 0; j < ml.get_size(); ++j) {
      auto tupl = std::any_cast<std::tuple<int, std::size_t>>(results[j].get());

      auto score = -std::get<0>(tupl);
      auto result = std::get<1>(tupl);

      nodecount += result;

      ml.set_heuristic(j, score);
    }
    tp.cancel_pending();
    std::cout << i + 1 << '\t' << c.elapsed_ms() / 1000.0 << '\t' << nodecount << '\n';
  }
  tp.finish();

  ml.sort();

  for (int i = 0; i < ml.get_size(); ++i) {
    std::cout << '\t' << ml[i].to_string() << '\t' << ml[i].get_heuristic() / (100.0) << '\n';
  }
  return std::make_tuple(ml[0], nodecount);
}

}  // namespace BeepBoop
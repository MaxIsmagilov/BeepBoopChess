#pragma once

#include <chrono>

namespace BobChess
{

class Clock
{
 public:
  bool time_up(double milliseconds) const;
  double elapsed() const;
  void start();
  void stop();

 private:
  std::chrono::_V2::system_clock::time_point m_start_time;
};

}  // namespace BobChess

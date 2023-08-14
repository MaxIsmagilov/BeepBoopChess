#pragma once

#include <chrono>

namespace BeepBoop
{

class Clock
{
 public:
  bool time_up(double milliseconds) const;
  double elapsed_ms() const;
  double elapsed_ns() const;
  void start();
  void stop();

 private:
  std::chrono::_V2::system_clock::time_point m_start_time;
};

}  // namespace BeepBoop

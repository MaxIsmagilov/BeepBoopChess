#include "timekeeping.hpp"

namespace BobChess
{

bool Clock::time_up(double milliseconds) const {
  auto stop = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - m_start_time);
  return (time.count() > milliseconds);
}
double Clock::elapsed() const {
  auto stop = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - m_start_time);
  return time.count();
}

void Clock::start() { m_start_time = std::chrono::high_resolution_clock::now(); }

void Clock::stop() {}

}  // namespace BobChess

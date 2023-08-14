#include <any>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>

namespace BeepBoop
{

class ThreadPool
{
 private:
  std::mutex m;
  std::condition_variable v;
  std::deque<std::packaged_task<void()>> pending;
  std::vector<std::future<void>> finished;

  void thread_task() {
    while (true) {
      std::packaged_task<void()> f;
      {
        std::unique_lock<std::mutex> l(m);
        if (pending.empty()) {
          v.wait(l, [&] { return !pending.empty(); });
        }
        f = std::move(pending.front());
        pending.pop_front();
      }
      if (!f.valid()) {
        return;
      }
      f();
    }
  }

 public:
  template <typename F, typename... Params>
  std::future<std::any> queue(F&& f, Params&&... args) {
    std::packaged_task<std::any()> p(std::bind(std::forward<F>(f), std::forward<Params>(args)...));

    auto r = p.get_future();

    std::unique_lock<std::mutex> l(m);
    pending.emplace_back(std::move(p));

    v.notify_one();

    return r;  // return the future result of the task
  }

  void go(std::size_t N = 1, std::launch policy = std::launch::async) {
    for (std::size_t i = 0; i < N; ++i) {
      finished.push_back(std::async(policy, [this] { thread_task(); }));
    }
  }

  void abort() {
    cancel_pending();
    finish();
  }

  // cancel_pending() merely cancels all non-started tasks:
  void cancel_pending() {
    std::unique_lock<std::mutex> l(m);
    pending.clear();
  }

  // finish enques a "stop the thread" message for every thread, then waits for them:
  void finish() {
    {
      std::unique_lock<std::mutex> l(m);
      for ([[maybe_unused]] auto&& unused : finished) {
        pending.push_back({});
      }
    }
    v.notify_all();
    finished.clear();
  }
  ~ThreadPool() { finish(); }
};

}  // namespace BeepBoop

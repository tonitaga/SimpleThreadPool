#ifndef THREADPOOL_INCLUDE_THREAD_POOL_UNBOUNDED_BLOCKING_QUEUE_H
#define THREADPOOL_INCLUDE_THREAD_POOL_UNBOUNDED_BLOCKING_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

namespace pool {

template <typename T, typename Container = std::deque<T>>
class unbounded_blocking_queue final {
public:
  using value_type = T;

public:
  template<typename ...Args>
  void emplace_back(Args &&...args) {
	std::lock_guard guard(mutex_);
	queue_.emplace_back(std::forward<Args>(args)...);
	queue_non_empty_.notify_one();
  }

  template<typename ...Args>
  void emplace_front(Args &&...args) {
	std::lock_guard guard(mutex_);
	queue_.emplace_front(std::forward<Args>(args)...);
	queue_non_empty_.notify_one();
  }

  value_type wait_and_pop() {
	std::unique_lock lock(mutex_);

	queue_non_empty_.wait(lock, [this]{
	  return !queue_.empty();
	});

	value_type value = queue_.front();
	queue_.pop_front();
	return value;
  }

private:
  Container queue_; // Guarded by mutex_
  std::mutex mutex_;
  std::condition_variable queue_non_empty_;
};

} // namespace pool end

#endif //THREADPOOL_INCLUDE_THREAD_POOL_UNBOUNDED_BLOCKING_QUEUE_H

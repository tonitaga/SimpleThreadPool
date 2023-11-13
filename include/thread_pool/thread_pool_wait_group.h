#ifndef THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_WAIT_GROUP_H
#define THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_WAIT_GROUP_H

#include <mutex>
#include <condition_variable>

namespace pool {

class thread_pool_wait_group {
public:
  void add() {
	std::lock_guard guard(mutex_);
	waiters_count_ += 1;
	zero_waiters_.notify_one();
  }

  void sub() {
	std::lock_guard guard(mutex_);
	waiters_count_ -= 1;
	zero_waiters_.notify_one();
  }

  void wait_zero() {
	std::unique_lock lock(mutex_);
	if (waiters_count_ == 0)
	  return;

	zero_waiters_.wait(lock, [this]{
	  return waiters_count_ == 0;
	});
  }

private:
  int waiters_count_ = 0; // Guarded by mutex_
  std::mutex mutex_;
  std::condition_variable zero_waiters_;
};

}

#endif //THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_WAIT_GROUP_H

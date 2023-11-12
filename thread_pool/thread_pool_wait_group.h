#ifndef SIMPLE_THREAD_POOL_THREAD_POOL_WAIT_GROUP_H_
#define SIMPLE_THREAD_POOL_THREAD_POOL_WAIT_GROUP_H_

#include <mutex>
#include <condition_variable>

namespace pool::internal {
    class ThreadPoolWaitGroup {
    public:
        void add_waiter() {
            std::lock_guard guard(mutex_);
            waiters_++;
        }

        void remove_waiter() {
            std::lock_guard guard(mutex_);
            waiters_--;
            if (waiters_ == 0)
                no_waiters_.notify_one();
        }

        void wait_no_waiters() {
            std::unique_lock lock(mutex_);
            no_waiters_.wait(lock, [this] { return waiters_ == 0; });
        }

    private:
        std::mutex mutex_;
        std::size_t waiters_ = 0; // Guarded by mutex_
        std::condition_variable no_waiters_;
    };
}

#endif //SIMPLE_THREAD_POOL_THREAD_POOL_WAIT_GROUP_H_
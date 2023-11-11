#ifndef SIMPLE_THREAD_POOL_THREAD_SAFETY_BLOCKING_QUEUE_H_
#define SIMPLE_THREAD_POOL_THREAD_SAFETY_BLOCKING_QUEUE_H_

#include <mutex>
#include <queue>
#include <condition_variable>

namespace pool {
    template <typename T, typename Container = std::deque<T>>
    class ThreadSafetyBlockingQueue {
    public:
        using value_type = typename Container::value_type;
        using size_type = typename Container::size_type;

    public:
        ThreadSafetyBlockingQueue() noexcept = default;

        ThreadSafetyBlockingQueue(const ThreadSafetyBlockingQueue &other) {
            std::lock_guard guard(other.mutex_);
            queue_ = other.queue_;
        }

    public:
        void push(const value_type &value) {
            std::lock_guard guard(mutex_);
            queue_.push_back(value);
            queue_non_empty_.notify_one();
        }

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

        void push(value_type &&value) {
            std::lock_guard guard(mutex_);
            queue_.push_back(std::move(value));
            queue_non_empty_.notify_one();
        }

        std::shared_ptr<value_type> wait_and_pop() {
            std::unique_lock lock(mutex_);
            queue_non_empty_.wait(lock, [this] { return !queue_.empty(); });

            std::shared_ptr value = std::make_shared<value_type>(queue_.front());
            queue_.pop_front();
            return value;
        }

        bool try_pop(value_type &value) {
            std::lock_guard guard(mutex_);
            if (queue_.empty())
                return false;
            value = queue_.front();
            queue_.pop_front();
        }

        std::shared_ptr<value_type> try_pop() {
            std::lock_guard guard(mutex_);
            if (queue_.empty())
                return nullptr;
            std::shared_ptr value = std::make_shared<value_type>(queue_.front());
            queue_.pop_front();
            return value;
        }

        bool empty() {
            std::lock_guard guard(mutex_);
            return queue_.empty();
        }

    private:
        Container queue_; // Guarded by mutex_
        std::mutex mutex_;
        std::condition_variable queue_non_empty_;
    };
}

#endif //SIMPLE_THREAD_POOL_THREAD_SAFETY_BLOCKING_QUEUE_H_

#ifndef THREADPOOL_THREAD_POOL_H
#define THREADPOOL_THREAD_POOL_H

#include <thread_pool/thread_pool_task.h>
#include <thread_pool/thread_safety_blocking_queue.h>

#include <vector>
#include <thread>

namespace pool {
    /**
     * @class ThreadPool
     * @brief A thread pool implementation for executing tasks concurrently.
     *
     * The ThreadPool class provides a mechanism to execute tasks concurrently by
     * distributing them among a fixed number of worker threads. Users can submit
     * tasks to the thread pool, wait for their completion, stop the thread pool,
     * and launch the workers again.
     */
    class ThreadPool {
    public:
        /**
         * @brief Constructs a ThreadPool object with the specified number of worker threads.
         * @param workers_count The number of worker threads to create in the thread pool.
         *                      If not provided, it defaults to the number of hardware threads.
         */
        explicit ThreadPool(std::size_t workers_count = std::thread::hardware_concurrency());

        ~ThreadPool() noexcept = default;

    public:

        /**
         * @brief Submits a task to the thread pool for execution.
         * @param task The task to be executed by the thread pool.
         *
         * The task is added to the queue of tasks to be executed by the worker threads.
         * The task should be a callable object that takes no arguments and returns no value.
         */
        void submit(ThreadPoolTask task);

        /**
         * @brief Waits for all submitted tasks to complete.
         *
         * This function blocks until all tasks in the queue have been executed and completed.
         * It is useful when you need to ensure that all tasks have finished before proceeding
         * with the rest of the program.
         */
        void wait();

        /**
         * @brief Stops the thread pool without waiting for all tasks to complete.
         *
         * This function stops the thread pool by adding empty tasks to the queue for each worker thread.
         * The worker threads will execute these empty tasks and exit the routine_work() loop, effectively
         * stopping the thread pool. The function then waits for all worker threads to join, ensuring that
         * all tasks have completed before returning.
         */
        void stop();

        void launch_workers_again();

    private:
        std::vector<std::thread> workers_;
        ThreadSafetyBlockingQueue<internal::ThreadPoolTask> tasks_;

    private:
        void create_workers(std::size_t workers_count);
        void routine_work();
        void join_workers();
        void launch_workers();
    };

    ThreadPool::ThreadPool(std::size_t workers_count) {
        create_workers(workers_count);
    }

    void ThreadPool::submit(ThreadPoolTask task) {
        tasks_.emplace_back(task, internal::kSubmitTask);
    }

    void ThreadPool::wait() {
        while (!tasks_.empty()) {}
    }

    void ThreadPool::stop() {
        static auto empty_task = []{};
        for (std::size_t i = 0; i != workers_.size(); ++i)
            tasks_.emplace_front(empty_task, internal::kStopTask);

        join_workers();
    }

    void ThreadPool::launch_workers_again() {
        join_workers();
        launch_workers();
    }

    void ThreadPool::create_workers(std::size_t workers_count) {
        if (workers_count == 0)
            throw std::logic_error("Thread pool with 0 threads is illegal instruction");

        workers_.reserve(workers_count);
        for (std::size_t i = 0; i != workers_count; ++i) {
            workers_.emplace_back([this] {
                routine_work();
            });
        }
    }

    void ThreadPool::routine_work() {
        while (true) {
            auto [Task, task_type] = *tasks_.wait_and_pop();
            if (task_type == internal::kStopTask)
                break;

            Task();
        }
    }

    void ThreadPool::join_workers() {
        for (auto &worker : workers_)
            if (worker.joinable())
                worker.join();
    }

    void ThreadPool::launch_workers() {
        for (auto &worker : workers_) {
            worker = std::thread([&]{
                routine_work();
            });
        }
    }
}

#endif //THREADPOOL_THREAD_POOL_H

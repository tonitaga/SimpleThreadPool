#ifndef SIMPLE_THREAD_POOL_THREAD_POOL_TASK_H_
#define SIMPLE_THREAD_POOL_THREAD_POOL_TASK_H_

#include <functional>

namespace pool {
    using ThreadPoolTask = std::function<void()>;

    namespace internal {
        enum ThreadPoolTaskType {
            kSubmitTask,
            kStopTask
        };

        struct ThreadPoolTask {
        public:
            pool::ThreadPoolTask task;
            ThreadPoolTaskType type;

        public:
            ThreadPoolTask(pool::ThreadPoolTask task, ThreadPoolTaskType type) : task(std::move(task)), type(type) {};
        };
    }
}

#endif //SIMPLE_THREAD_POOL_THREAD_POOL_TASK_H_

#ifndef THREADPOOL_THREAD_POOL_TASK_H
#define THREADPOOL_THREAD_POOL_TASK_H

#include <functional>

namespace pool {
    using ThreadPoolTask = std::function<void()>;

    namespace internal {
        enum ThreadPoolTaskType {
            kSubmitTask,
            kStopTask
        };

        using ThreadPoolTask = std::pair<ThreadPoolTask, ThreadPoolTaskType>;
    }
}

#endif //THREADPOOL_THREAD_POOL_TASK_H

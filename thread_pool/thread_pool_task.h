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

        using ThreadPoolTask = std::pair<ThreadPoolTask, ThreadPoolTaskType>;
    }
}

#endif //SIMPLE_THREAD_POOL_THREAD_POOL_TASK_H_

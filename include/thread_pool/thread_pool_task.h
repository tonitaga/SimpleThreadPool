#ifndef THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_TASK_H
#define THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_TASK_H

#include <functional>
#include <thread_pool/thread_pool_types.h>

namespace pool {

using pool_task = std::function<void()>;

namespace internal {

struct thread_pool_task {
  pool_task task;
  task_type type;

  thread_pool_task(pool_task task, task_type type)
	  : task(std::move(task)), type(type) {};
};

} // namespace internal end
} // namespace pool end

#endif //THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_TASK_H

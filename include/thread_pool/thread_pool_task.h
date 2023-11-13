//
//		Simple thread pool implementation
//		Supports subscribing for void() tasks,
//		changing priority of task, and control destructor
//      implementation, also supports waiting all tasks
//		before submit for another task
//
//		Simple thread pool written on C++17 Standard
//		Also this project contains Thread Safe Blocking Queue
//
//		[Author]  Gubaydullin Nurislam aka tonitaga
//		[Gmail]   gubaydullin.nurislam@gmail.com
//      [Telegram] @tonitaga
//

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

/*
*	Simple thread pool implementation
*	Supports subscribing for void() tasks,
*	changing priority of task, and control destructor
*	implementation, also supports waiting all tasks
*	before submit for another task
*
*	Simple thread pool written on C++17 Standard
*	Also this project contains Thread Safe Blocking Queue
*
*	[Author]  Gubaydullin Nurislam aka tonitaga
*	[Gmail]   gubaydullin.nurislam@gmail.com
*	[Telegram] @tonitaga
*/

#ifndef THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_TYPES_H
#define THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_TYPES_H

namespace pool {

/**
 * @enum
 * thread_pool destructor policy types
 * @param waiting - waiting all tasks finish in destructor
 * @param aborting - abort incomplete tasks in destructor
 */
enum class stop_type {
  waiting,
  aborting
};

/**
 * @enum submit_type
 * thread_pool::submit method launch policy
 * @param usual - add task to the end of task-queue
 * @param immediate - add task to the begin of task-queue
 */
enum class submit_type {
  usual,
  immediate
};

namespace internal {

enum class task_type {
  user_submit,
  internal_stop
};

} // namespace internal end
} // namespace pool end

#endif //THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_TYPES_H

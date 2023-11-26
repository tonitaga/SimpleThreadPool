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

#ifndef THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_H
#define THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_H

#include <thread>
#include <vector>
#include <stdexcept>

#include <thread_pool/thread_pool_task.h>
#include <thread_pool/thread_pool_types.h>
#include <thread_pool/thread_pool_wait_group.h>
#include <thread_pool/unbounded_blocking_queue.h>

namespace pool {

class thread_pool {
public:
  thread_pool(std::size_t workers, stop_type type)
	  : stop_type_(type) {
	if (workers == 0)
	  throw std::logic_error("worker == 0");

	workers_.reserve(workers);
	for (std::size_t i = 0; i != workers; ++i) {
	  workers_.emplace_back([this] {
		while (true) {
		  auto [task, task_type] = tasks_.wait_and_pop();
		  if (task_type == internal::task_type::internal_stop)
			break;

		  task();
		  wait_group_.sub();
		}
	  });
	}
  }

  explicit thread_pool(std::size_t workers = std::thread::hardware_concurrency())
	  : thread_pool(workers, stop_type::aborting) {};

  ~thread_pool() {
	stop();
  }

public:
  void submit(pool_task task, submit_type type) {
	if (type == submit_type::usual) {
	  wait_group_.add();
	  tasks_.emplace_back(std::move(task),
						  internal::task_type::user_submit);
	} else {
	  wait_group_.add();
	  tasks_.emplace_front(std::move(task),
						   internal::task_type::user_submit);
	}
  }

  void submit(pool_task task) {
	submit(std::move(task), submit_type::usual);
  }

  void wait() {
	wait_group_.wait_zero();
  }

private:
  std::vector<std::thread> workers_;
  unbounded_blocking_queue<internal::thread_pool_task> tasks_;
  thread_pool_wait_group wait_group_;
  stop_type stop_type_;

private:
  void stop() {
	const std::size_t size = workers_.size();
	if (stop_type_ == stop_type::aborting) {
	  for (std::size_t i = 0; i != size; ++i)
		tasks_.emplace_front([]{}, internal::task_type::internal_stop);
	} else {
	  for (std::size_t i = 0; i != size; ++i)
		tasks_.emplace_back([]{}, internal::task_type::internal_stop);
	}

	for (auto &worker : workers_) {
	  if (worker.joinable())
		worker.join();
	}
  }
};

}

#endif //THREADPOOL_INCLUDE_THREAD_POOL_THREAD_POOL_H

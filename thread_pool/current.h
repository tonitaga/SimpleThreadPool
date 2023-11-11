#ifndef SIMPLE_THREAD_POOL_CURRENT_H_
#define SIMPLE_THREAD_POOL_CURRENT_H_

#include <thread_pool/thread_pool.h>

namespace pool {
    ThreadPool &currentThreadPool() {
        static ThreadPool thread_pool(defined_workers_count);
        return thread_pool;
    }
}

#endif //SIMPLE_THREAD_POOL_CURRENT_H_

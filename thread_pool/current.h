#ifndef THREADPOOL_CURRENT_H
#define THREADPOOL_CURRENT_H

#include <thread_pool/thread_pool.h>

namespace pool {
    ThreadPool &currentThreadPool() {
        static ThreadPool thread_pool(defined_workers_count);
        return thread_pool;
    }
}

#endif //THREADPOOL_CURRENT_H

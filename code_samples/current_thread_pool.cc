#include <thread_pool/current.h>

#include <chrono>
#include <iostream>

#define THREAD_ID() std::this_thread::get_id()

using namespace std::chrono_literals;

void Task() {
    std::this_thread::sleep_for(100ms);
    std::cout << THREAD_ID() << ") #5" << std::endl;
}

int main() {
    using namespace pool;

    currentThreadPool().submit([]{
        std::cout << THREAD_ID() << ") #1" << std::endl;
    });

    currentThreadPool().submit([]{
        std::this_thread::sleep_for(20ms);
        std::cout << THREAD_ID() << ") #2" << std::endl;
    });

    currentThreadPool().submit([]{
        std::this_thread::sleep_for(50ms);
        std::cout << THREAD_ID() << ") #3" << std::endl;
    });

    pool::ThreadPoolTask task = []{
        std::this_thread::sleep_for(70ms);
        std::cout << THREAD_ID() << ") #4" << std::endl;
    };

    currentThreadPool().submit(std::move(task));
    currentThreadPool().submit(Task);

    currentThreadPool().wait();

    return EXIT_SUCCESS;
}
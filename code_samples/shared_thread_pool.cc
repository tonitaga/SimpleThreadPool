#include <thread_pool/shared.h>

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

    sharedThreadPool().submit([]{
        std::cout << THREAD_ID() << ") #1" << std::endl;
    });

    sharedThreadPool().submit([]{
        std::this_thread::sleep_for(20ms);
        std::cout << THREAD_ID() << ") #2" << std::endl;
    });

    sharedThreadPool().submit([]{
        std::this_thread::sleep_for(50ms);
        std::cout << THREAD_ID() << ") #3" << std::endl;
    });

    pool::ThreadPoolTask task = []{
        std::this_thread::sleep_for(70ms);
        std::cout << THREAD_ID() << ") #4" << std::endl;
    };

    sharedThreadPool().submit(std::move(task));
    sharedThreadPool().submit(Task);

    sharedThreadPool().wait();

    return EXIT_SUCCESS;
}
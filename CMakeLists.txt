cmake_minimum_required(VERSION 3.5...3.16)
project(ThreadPool)

include(cmake/definitions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -Wextra")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(
    ${PROJECT_NAME}
        code_samples/thread_pool.cc
)

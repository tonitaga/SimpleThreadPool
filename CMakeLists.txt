cmake_minimum_required(VERSION 3.26)
project(ThreadPool)

include(cmake/definitions.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Werror -Wextra")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    ${PROJECT_NAME}
        code_samples/thread_pool.cc
)

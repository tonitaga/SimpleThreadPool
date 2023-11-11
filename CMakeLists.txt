cmake_minimum_required(VERSION 3.26)
project(ThreadPool)

include(cmake/definitions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
    ${PROJECT_NAME}
        code_samples/usage.cc
)

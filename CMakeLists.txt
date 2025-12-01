cmake_minimum_required(VERSION 3.15)
project(matching_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -march=native -DNDEBUG")

include_directories(include)

# ======== MAIN ENGINE =========
add_executable(engine
    src/main.cpp
    src/matching_engine.cpp
    src/book.cpp
    src/order_pool.cpp
    src/execution_log.cpp
)

# ======== BENCHMARK ===========
add_executable(bench
    src/bench.cpp
    src/matching_engine.cpp
    src/book.cpp
    src/order_pool.cpp
    src/execution_log.cpp
)

# ======== HTTP SERVER =========
add_executable(server
    src/server.cpp
    src/matching_engine.cpp
    src/book.cpp
    src/order_pool.cpp
    src/execution_log.cpp
)

target_include_directories(server PRIVATE include)


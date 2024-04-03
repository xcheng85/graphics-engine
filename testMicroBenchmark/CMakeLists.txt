cmake_minimum_required(VERSION 3.14)
project(testMicroBenchmark)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Google Benchmark
include(../cmake/GoogleBenchmark.cmake)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} benchmark::benchmark)

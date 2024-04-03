# Disable the Google Benchmark requirement on Google Test
set(BENCHMARK_ENABLE_TESTING NO)
set(DBENCHMARK_DOWNLOAD_DEPENDENCIES ON)

include(FetchContent)

FetchContent_Declare(googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
)

FetchContent_MakeAvailable(googlebenchmark)

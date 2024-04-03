#include "mem-debugger.h"

int main()
{
    MemDebugger memDebugger{"default"};
    // 528 byte by default
    // 1 block per chunk, default do_allocate(): 32
    // 8 block per chunk, default do_allocate(): 64
    std::pmr::unsynchronized_pool_resource unsync_pool({
                                                           6400,
                                                           6400, // 8kb
                                                       },
                                                       &memDebugger);
    // int: 32 bytes

    std::pmr::vector<int> v2{&unsync_pool};
    v2.emplace_back(1);
    v2.emplace_back(2);
    v2.emplace_back(3);
    v2.emplace_back(4);
    // v2.emplace_back(3);
    return 0;
}
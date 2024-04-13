#include <assert.h>
#include <format>
#include "tlsf.h"
#include "memory.h"

static MemoryManager g_memoryManager;
// 64mb + extra internal ds overhead
static size_t g_memoryPoolSize = 64 * 1024 * 1024 + tlsf_size() + 8;

void StackLinearAllocator::init(size_t size)
{
    _memory = (std::byte *)malloc(size);
    _totalSize = size;
}

void StackLinearAllocator::teardown()
{
    free(_memory);
}

// game engine architecture
size_t alignMemoryAddress(size_t memoryAddress, size_t align)
{
    const auto mask = align - 1;
    // quickiest way of detect power of 2
    assert((align & mask) == 0); // power of 2;
    return (memoryAddress + mask) & ~mask;
}

// 8bit integer: aligned to any address
// int, float: 4 byte alignement
// 128 bit SIMD vector (4 * 32): 16 byte alignment
void *StackLinearAllocator::allocate(size_t bytes, size_t alignment)
{
    assert(bytes > 0);
    const auto newAlignedMemoryAddress = alignMemoryAddress(_allocatedSize, alignment);
    assert(newAlignedMemoryAddress < _totalSize);

    const auto newAllocatedSize = newAlignedMemoryAddress + bytes;

    if (newAllocatedSize > _totalSize)
    {
        std::cerr << "StackAllocator overflow\n";
        assert(false);
        return nullptr;
    }

    _allocatedSize = newAllocatedSize;
    return _memory + newAlignedMemoryAddress;
}

// pay attention to the deallocate order
void StackLinearAllocator::deallocate(void *pointer)
{
    // check boundary of pointer
    assert(pointer >= _memory);
    assert(pointer < _memory + _totalSize);
    assert(pointer < _memory + _allocatedSize);
    const auto marker = (std::byte *)pointer - _memory;
    _allocatedSize = marker;
}

void StackLinearAllocator::freeToMarker(size_t marker)
{
    const size_t difference = marker - _allocatedSize;
    if (difference > 0)
    {
        _allocatedSize = marker;
    }
}

// pool allocator
// different size of object in the pool
// Two-Level Segregated Fit memory allocator implementation.
// tlsf is able to handle multiple memory pools
//
void HeapPoolAllocator::init(size_t size)
{
    _memory = (std::byte *)malloc(size);
    _totalSize = size;
    // caller pass in the address to let tlsf manage
    _tlsfHandle = tlsf_create_with_pool(_memory, size);
}

struct MemoryStatistics
{
    size_t allocatedByteSize;
    size_t totalByteSize;

    size_t allocatedBlockCount;

    void add(size_t usedByteSize)
    {
        if (usedByteSize)
        {
            allocatedByteSize += usedByteSize;
            ++allocatedBlockCount;
        }
    }
};

// struct MemoryStatistics
// tlsf memory debugger function signature
// ptr: unrelease block ptr
// size: size of block
// used: this block is used.
// *user: custom
void exitWalker(void *ptr, size_t size, int used, void *user)
{
    MemoryStatistics *stats = (MemoryStatistics *)user;
    stats->add(used ? size : 0);
    if (used)
    {
        std::cout << std::format("Found in-use block addr: {}, size: {}\n", ptr, size);
    }
}

void HeapPoolAllocator::teardown()
{
    // Check memory at the application exit.
    MemoryStatistics stats{0, _totalSize};
    auto poolHandle = tlsf_get_pool(_tlsfHandle);
    tlsf_walk_pool(poolHandle, exitWalker, (void *)&stats);
    // check memory leakage
    assert(stats.allocatedByteSize == 0);
    tlsf_destroy(_tlsfHandle);
    free(_memory);
}

void *HeapPoolAllocator::allocate(size_t size, size_t alignment)
{
    std::cout << std::format("allocate {} bytes with alignment {} ...\n", size, alignment);
    void *allocatedMemory = alignment == 1 ? tlsf_malloc(_tlsfHandle, size) : tlsf_memalign(_tlsfHandle, alignment, size);
    auto blockSize = tlsf_block_size(allocatedMemory);
    _allocatedSize += blockSize;
    return allocatedMemory;
}

void HeapPoolAllocator::deallocate(void *pointer)
{
    std::cout << std::format("deallocate at address {} ...\n", pointer);
    auto blockSize = tlsf_block_size(pointer);
    _allocatedSize -= blockSize;
    tlsf_free(_tlsfHandle, pointer);
}

MemoryManager *MemoryManager::instance()
{
    return &g_memoryManager;
}

void MemoryManager::init(void *configuration)
{
    std::cout << "MemoryManager::init\n";
    _heapPoolAllocator.init(g_memoryPoolSize);
    _linearStackAllocator.init(g_memoryPoolSize);
}

void MemoryManager::teardown()
{
    _heapPoolAllocator.teardown();
    std::cout << "MemoryManager::teardown\n";
}

void MemoryManager::test()
{
    StackLinearAllocator sla;
    sla.init(1024);

    // Allocate 3 times
    void *a1 = sla.allocate(16, 4);
    void *a2 = sla.allocate(20, 4);
    void *a3 = sla.allocate(10, 4);

    // Free based on size
    sla.deallocate(a3);
    void *a4 = sla.allocate(10, 4);
    assert(a3 == a4);

    void *outofbounds = sla.allocate(1025, 4);
}
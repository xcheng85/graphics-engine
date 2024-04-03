#include "pch.h"
#include "memory.h"
#include "tlsf.h"

namespace ge
{
    // static vs dynamic.
    // static: size must be compile-time
    // malloc vs new
    // “new” does call the constructor of a class whereas “malloc()” does not.
    void StackAllocator::init(size_t size)
    {
        _memory = (std::byte *)malloc(size);
        _totalSize = size;
    }

    void StackAllocator::shutdown()
    {
        free(_memory);
    }

    // game engine architecture
    size_t alignMemoryAddress(size_t memoryAddress, size_t align)
    {
        const auto mask = align - 1;
        // quickiest way of detect power of 2
        GE_ASSERT((align & mask) == 0); // power of 2;
        return (memoryAddress + mask) & ~mask;
    }

    // 8bit integer: aligned to any address
    // int, float: 4 byte alignement
    // 128 bit SIMD vector (4 * 32): 16 byte alignment
    void *StackAllocator::allocate(size_t bytes, size_t alignment)
    {
        GE_ASSERT(bytes > 0);
        const auto newAlignedMemoryAddress = alignMemoryAddress(_allocatedSize, alignment);
        GE_ASSERT(newAlignedMemoryAddress < _totalSize);

        const auto newAllocatedSize = newAlignedMemoryAddress + bytes;

        if (newAllocatedSize > _totalSize)
        {
            GE_ERROR("StackAllocator overflow");
            GE_ASSERT(false);
            return nullptr;
        }

        _allocatedSize = newAllocatedSize;
        return _memory + newAlignedMemoryAddress;
    }

    // pay attention to the deallocate order
    void StackAllocator::deallocate(void *pointer)
    {
        // check boundary of pointer
        GE_ASSERT(pointer >= _memory);
        GE_ASSERT(pointer < _memory + _totalSize);
        GE_ASSERT(pointer < _memory + _allocatedSize);
        const auto marker = (std::byte *)pointer - _memory;
        _allocatedSize = marker;
    }

    // pool allocator
    // different size of object in the pool
    // Two-Level Segregated Fit memory allocator implementation.
    // tlsf is able to handle multiple memory pools
    //
    void HeapAllocator::init(size_t size)
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
    }; // struct MemoryStatistics
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

    void HeapAllocator::shutdown()
    {
        // Check memory at the application exit.
        MemoryStatistics stats{0, _totalSize};
        auto poolHandle = tlsf_get_pool(_tlsfHandle);
        tlsf_walk_pool(poolHandle, exitWalker, (void *)&stats);
        // check memory leakage
        GE_ASSERT(stats.allocatedByteSize == 0);
        tlsf_destroy(_tlsfHandle);
        free(_memory);
    }

    void *HeapAllocator::allocate(size_t size, size_t alignment)
    {
        void *allocatedMemory = alignment == 1 ? tlsf_malloc(_tlsfHandle, size) : tlsf_memalign(_tlsfHandle, alignment, size);
        auto blockSize = tlsf_block_size(allocatedMemory);
        _allocatedSize += blockSize;
        return allocatedMemory;
    }

    void HeapAllocator::deallocate(void *pointer)
    {
        auto blockSize = tlsf_block_size(pointer);
        _allocatedSize -= blockSize;
        tlsf_free(_tlsfHandle, pointer);
    }
}
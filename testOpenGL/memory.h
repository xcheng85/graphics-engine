#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class IMemoryAllocator
{
public:
    virtual ~IMemoryAllocator() {}
    virtual void *allocate(size_t size, size_t alignment) = 0;
    virtual void deallocate(void *pointer) = 0;
};

// careful about the deallocate order
class StackLinearAllocator : public IMemoryAllocator
{
public:
    void init(size_t size);
    void teardown();

    void *allocate(size_t size, size_t alignment) override;
    void deallocate(void *pointer) override;

private:
    // marker: boundary of used and unused memory
    inline size_t getMarker() const noexcept
    {
        return _allocatedSize;
    }
    // // rolls the stack back to the previous marker
    void freeToMarker(size_t marker);
    inline void clear()
    {
        _allocatedSize = 0;
    }

    // byte is better alternative as uchar8
    std::byte *_memory{nullptr};
    size_t _totalSize{0};
    size_t _allocatedSize{0};
};

class HeapPoolAllocator : public IMemoryAllocator
{
public:
    void init(size_t size);
    void teardown();

    void *allocate(size_t size, size_t alignment) override;
    void deallocate(void *pointer) override;

private:
    void *_tlsfHandle{nullptr};
    std::byte *_memory{nullptr};
    size_t _allocatedSize{0};
    size_t _totalSize{0};
};

class MemoryManager
{
public:
    static MemoryManager *instance();
    void init(void *configuration);
    void teardown();

    inline StackLinearAllocator &stackLinearAllocator()
    {
        return _linearStackAllocator;
    }
    inline HeapPoolAllocator &heapPoolAllocator()
    {
        return _heapPoolAllocator;
    }

    void test();

private:
    StackLinearAllocator _linearStackAllocator;
    HeapPoolAllocator _heapPoolAllocator;
};

// for stl container
template <class T>
class HeapAllocator
{
public:
    using value_type = T;
    using arena_type = IMemoryAllocator;
    HeapAllocator(IMemoryAllocator *arena) noexcept : _arena{arena}
    {
    }
    template <class U>
    HeapAllocator(const HeapAllocator<U> &other) noexcept
        : _arena{other._arena}
    {
    }
    HeapAllocator(const HeapAllocator &) = default;
    HeapAllocator &operator=(const HeapAllocator &) = default;

    template <class U>
    struct rebind
    {
        using other = HeapAllocator<U>;
    };
    T *allocate(size_t n)
    {
        return reinterpret_cast<T *>(_arena->allocate(n * sizeof(T), _alignment));
    }
    void deallocate(T *p, size_t n) noexcept
    {
        _arena->deallocate(p);
    }
    template <class U>
    auto operator==(const HeapAllocator<U> &other) const noexcept
    {
        return _arena == other._arena;
    }
    template <class U>
    auto operator!=(const HeapAllocator<U> &other) const noexcept
    {
        return !(*this == other);
    }
    template <class U>
    friend struct HeapAllocator;

private:
    // at least as strict (as large) as that of every scalar type
    // std::max_align_t is usually synonymous with the largest scalar type, which is long double
    static constexpr size_t _alignment = alignof(std::max_align_t);
    IMemoryAllocator *_arena;
};

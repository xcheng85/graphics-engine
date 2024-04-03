#pragma once
#include "pch.h"
#include "module.h"

namespace ge
{
    // motivation:
    // heap allocation: kernel call, slow
    // continous access: cache friendly
    // 1. stack-based allocator
    // LIFO. order dep

    // 2. double-ended stack allocators

    // 3. single-frame allocator
    // clear every frame

    // 4. pool allocator

    // for non-stl container
    class IMemoryAllocator
    {
    public:
        virtual ~IMemoryAllocator() {}
        virtual void *allocate(size_t size, size_t alignment) = 0;
        virtual void deallocate(void *pointer) = 0;
    };

    class StackAllocator : public IMemoryAllocator
    {
    public:
        void init(size_t size);
        void shutdown();

        void *allocate(size_t size, size_t alignment) override;
        void deallocate(void *pointer) override;

    private:
        // // marker: boundary of used and unused memory
        // GE_INLINE size_t getMarker() const noexcept;
        // // rolls the stack back to the previous marker
        // void freeToMarker(sizet marker);
        // void clear();

        // byte is better alternative as uchar8
        std::byte *_memory{nullptr};
        size_t _totalSize{0};
        size_t _allocatedSize{0};
    };

    class HeapAllocator : public IMemoryAllocator
    {
    public:
        void init(size_t size);
        void shutdown();

        void *allocate(size_t size, size_t alignment) override;
        void deallocate(void *pointer) override;

    private:
        void *_tlsfHandle{nullptr};
        std::byte *_memory{nullptr};
        size_t _allocatedSize{0};
        size_t _totalSize{0};
    };

    class MemoryManager : public IModule
    {
    public:
        GE_SINGLETON(MemoryManager);
        void init(void *configuration) override;
        void shutdown() override;

        GE_INLINE StackAllocator &stackAllocator()
        {
            return _stackAllocator;
        }
        GE_INLINE HeapAllocator &heapAllocator()
        {
            return _heapAllocator;
        }

    private:
        StackAllocator _stackAllocator;
        HeapAllocator _heapAllocator;
    };

    // life-cyle is entire app
    static std::array<std::byte, 1024> buf2;
    static std::pmr::monotonic_buffer_resource appAllocator{buf2.data(), buf2.size(),
                                                            std::pmr::null_memory_resource()};

    //     // external linkage, let others to use it if needed
    //     constexpr size_t ALIGNMENT = alignof(std::max_align_t);

    //     // useful for vector of pointers
    //     // etc.
    //     template <size_t N>
    //     class LinearArena
    //     {
    //     public:
    //         Arena() noexcept : ptr_(buffer_) {}
    //         Arena(const Arena &) = delete;
    //         Arena &operator=(const Arena &) = delete;

    //         auto reset() noexcept { ptr_ = buffer_; }
    //         static constexpr auto size() noexcept { return N; }
    //         auto used() const noexcept { return static_cast<size_t>(ptr_ - buffer_); }
    //         auto allocate(size_t n) -> std::byte *;
    //         auto deallocate(std::byte *p, size_t n) noexcept -> void;

    //     private:
    //         static auto align_up(size_t n) noexcept -> size_t
    //         {
    //             return (n + (alignment - 1)) & ~(alignment - 1);
    //         }
    //         auto pointer_in_buffer(const std::byte *p) const noexcept -> bool
    //         {
    //             return std::uintptr_t(buffer_) <= std::uintptr_t(p) &&
    //                    std::uintptr_t(p) < std::uintptr_t(buffer_) + N;
    //         }
    //         alignas(alignment) std::byte buffer_[N];
    //         std::byte *ptr_{};
    //     };

    //     template <size_t N>
    //     auto Arena<N>::allocate(size_t n) -> std::byte *
    //     {
    //         const auto aligned_n = align_up(n);
    //         const auto available_bytes =
    //             static_cast<decltype(aligned_n)>(buffer_ + N - ptr_);
    //         if (available_bytes >= aligned_n)
    //         {
    //             auto *r = ptr_;
    //             ptr_ += aligned_n;
    //             return r;
    //         }
    //         return static_cast<std::byte *>(::operator new(n));
    //     }

    //     template <size_t N>
    //     auto Arena<N>::deallocate(std::byte *p, size_t n) noexcept -> void
    //     {
    //         if (pointer_in_buffer(p))
    //         {
    //             n = align_up(n);
    //             if (p + n == ptr_)
    //             {
    //                 ptr_ = p;
    //             }
    //         }
    //         else
    //         {
    //             ::operator delete(p);
    //         }
    //     }

    class traceResource : public std::pmr::memory_resource
    {
    public:
        explicit traceResource(std::string name,
                               std::pmr::memory_resource *up = std::pmr::get_default_resource())
            : _name{std::move(name)}, _upstream{up}
        {
        }

        void *do_allocate(size_t bytes, size_t alignment) override
        {
            std::cout << _name << " do_allocate(): " << bytes << '\n';
            void *ret = _upstream->allocate(bytes, alignment);
            return ret;
        }
        void do_deallocate(void *ptr, size_t bytes, size_t alignment) override
        {
            std::cout << _name << " do_deallocate(): " << bytes << '\n';
            _upstream->deallocate(ptr, bytes, alignment);
        }
        bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override
        {
            return this == &other;
        }

    private:
        std::string _name;
        std::pmr::memory_resource *_upstream;
    };
}
#include <string>
#include <iostream>
#include <memory_resource>

class MemDebugger : public std::pmr::memory_resource
{
public:
    explicit MemDebugger(std::string name,
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
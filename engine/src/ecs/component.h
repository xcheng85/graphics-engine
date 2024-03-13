#pragma once
#include "../pch.h"
#include "../core/memory.h"

extern std::pmr::monotonic_buffer_resource ge::appAllocator;
namespace ge::ecs
{
    using EntityID = size_t;
    using ComponentID = uint32_t;
    using ComponentSet = std::pmr::set<ComponentID>;
    using EntityIDSet = std::pmr::set<EntityID>;

    // value of this interface ?
    // support template and pass around without requiring its consumer to be template-based
    class IComponentCollection
    {
    public:
        GE_INLINE virtual ~IComponentCollection() = default;
        virtual void erase(EntityID) = 0;
    };

    template <typename T>
    struct Component
    {
        GE_INLINE Component(EntityID e, const T &d) : entityId{e}, data(d) {}
        GE_INLINE operator EntityID() const { return entityId; }
        EntityID entityId{INVALID_ID};
        T data;
    };

    // T: componentUID
    template <typename T>
    class ArrayComponentCollection : IComponentCollection
    {
    public:
        GE_INLINE T &push(EntityID eid, const T &data)
        {
            if (isExisted(eid))
            {
                // each entity has one instance of one type of component: camera, material
                return get(eid);
            }
            _components.emplace_back(Component<T>{eid, data});
            return _components.back().data;
        }

        GE_INLINE void erase(EntityID eid)
        {
            auto it = std::find(_components.begin(), _components.end(), [=](Component<T> &c)
                                { return c.entityId == eid; });
            if (it != _components.end())
            {
                _components.erase(it);
            }
        }

        GE_INLINE bool isExisted(EntityID eid)
        {
            return std::find(_components.begin(), _components.end(), eid) != _components.end();
        }

        GE_INLINE T &get(EntityID eid)
        {
            GE_ASSERT(isExisted(eid) && "entity out of range!");
            auto it = std::find(_components.begin(), _components.end(), [=](Component<T> &c)
                                { return c.entityId == eid; });
            return it->data;
        }

    private:
        // to be optimized
        std::pmr::vector<Component<T>> _components{&ge::appAllocator};
    };
}
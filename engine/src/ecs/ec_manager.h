#pragma once
#include "component.h"
#include "../pch.h"
#include "../core/memory.h"

// no static, external linkage
extern std::pmr::monotonic_buffer_resource ge::appAllocator;
namespace ge::ecs
{
    class EntityComponentManager
    {
    public:
        GE_INLINE void clear()
        {
            // allocated from stack memory, do nothing
            _components.clear();
            _ecs.clear();
            _nextEntityID = INVALID_ID;
        }

        GE_INLINE EntityID addEntity()
        {
            EntityID eid{++_nextEntityID};
            _ecs.emplace(eid, ComponentSet{&appAllocator});
            return eid;
        }

        GE_INLINE void destroy(EntityID eid)
        {
            for (auto &[_, a] : _components)
            {
                a->erase(eid);
            }
            // did it call the deallocate ?
            _ecs.erase(eid);
        }

        // another example
        // filterEntitiesByComponent
        template <typename Component>
        GE_INLINE EntityIDSet entitiesByComponent()
        {
            // cannot use memory arena here due to short-lived lifecycle
            EntityIDSet eids;
            for (auto &[eid, components] : _ecs)
            {
                if (components.count(typeId<Component>()))
                {
                    eids.insert(eid);
                    continue;
                }
            }
            return eids;
        }

        // return the ref component of an entity
        template <typename T>
        GE_INLINE T &getComponent(EntityID eid)
        {
            GE_ASSERT(_ecs.count(eid) && "out of range!");
            return getComponents<T>()->get(eid);
        }

        template <typename T, typename... Args>
        GE_INLINE T &addComponent(EntityID eid, Args &&...args)
        {
            GE_ASSERT(_ecs.count(eid) && "out of range!");
            auto component = T(std::forward<Args>(args)...);
            _ecs[eid].insert(typeId<T>());
            return getComponents<T>()->push(eid, component);
        }

        // remove component by type using meta-programming
        template <typename T>
        GE_INLINE void removeComponent(EntityID eid)
        {
            _ecs.at(eid).erase(typeId<T>());
            getComponents<T>()->erase(eid);
        }

        template <typename T>
        GE_INLINE bool hasComponent(EntityID eid)
        {
            return _ecs.count(eid) && _ecs.at(eid).count(typeId<T>());
        }

    private:
        // example using template meta replacing classic
        // instead of getComponent(entityId)
        // using compile to do the computing
        template <typename T>
        GE_INLINE ArrayComponentCollection<T> *getComponents()
        {
            ComponentID type = typeId<T>();
            if (_components.count(type))
            {
                // down-cast: parent to child
                return static_cast<ArrayComponentCollection<T> *>(_components.at(type));
            }
            // delayed init
            // allocate memory from the stack
            void *const p = appAllocator.allocate(sizeof(ArrayComponentCollection<T>), alignof(ArrayComponentCollection<T>));
            auto components = new (p) ArrayComponentCollection<T>();
            _components[type] = components;
            return components;
        }
        // using interface to overcome the template
        std::pmr::unordered_map<ComponentID, IComponentCollection *> _components{&appAllocator};
        std::pmr::unordered_map<EntityID, ComponentSet> _ecs{&ge::appAllocator};
        EntityID _nextEntityID{INVALID_ID};
    };
}
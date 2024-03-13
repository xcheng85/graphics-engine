#pragma once

#include "ec_manager.h"

namespace ge::ecs
{
    class Entity
    {
    public:
        GE_INLINE explicit Entity(EntityComponentManager *m) : _ecManager{m}
        {
            _eid = m->addEntity();
        }
        GE_INLINE explicit Entity(EntityID eid, EntityComponentManager *m) : _ecManager{m}, _eid{eid}
        {
        }
        GE_INLINE Entity() = delete;

        template <typename T, typename... Args>
        GE_INLINE T &addComponent(Args &&...args)
        {
            return _ecManager->addComponent<T>(_eid, std::forward<Args>(args)...);
        }

        template <typename T>
        GE_INLINE T &getComponent()
        {
            return _ecManager->getComponent<T>(_eid);
        }

        template <typename T>
        GE_INLINE void removeComponent()
        {
            _ecManager->removeComponent<T>(_eid);
        }

        template <typename T>
        GE_INLINE bool hasComponent()
        {
            return _ecManager->hasComponent<T>(_eid);
        }

        GE_INLINE auto id() { return _eid; }
        GE_INLINE void destroy() { _ecManager->destroy(_eid); }

    private:
        EntityID _eid{INVALID_ID};
        EntityComponentManager *_ecManager{nullptr};
    };
}
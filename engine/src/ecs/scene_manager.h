#pragma once
#include "../pch.h"
#include "system.h"
#include "ec_manager.h"
#include "entity.h"
#include "../core/memory.h"

extern std::pmr::monotonic_buffer_resource ge::appAllocator;
namespace ge::ecs
{

    class SceneManager
    {
    public:
        GE_INLINE SceneManager()
        {
            // register_system<ecs::script_system>();
            // register_system<ecs::rigidbody_system>();
            // register_system<ecs::collision_system>();
            // register_system<ecs::tilemap_renderer_system>();
            // register_system<ecs::frame_animation_system>();
            // register_system<ecs::sprite_renderer_system>();
            // register_system<ecs::text_renderer_system>();
        }

        GE_INLINE ~SceneManager()
        {
            // for (auto &s : _systems)
            // {
            //     FUSE_DELETE(s);
            // }
            // _registry.clear();
            // _systems.clear();
        }

        GE_INLINE Entity addEntity(const std::string &name)
        {
            Entity entity{&_ecManager};
            // add default must-have component here
            // entity.addComponent<info_component>().name = name;
            return entity;
        }

        GE_INLINE void update(float dt)
        {
            for (auto &s : _systems)
            {
                s->update(dt);
            }
        }

        GE_INLINE void start()
        {
            // load asset

            // background
            auto bg = addEntity("background");

            // start systems
            for (auto &sys : _systems)
            {
                sys->start();
            }
        }

    private:
        template <std::derived_from<ISystem> T>
        GE_INLINE void registerSystem()
        {
            void *const p = appAllocator.allocate(sizeof(T), alignof(T));
            auto system = new (p) T();
            system->startup(&_ecManager);
            this->_systems.emplace_back(system);
        }

        // pmr
        std::pmr::vector<ISystem *> _systems{&ge::appAllocator};
        EntityComponentManager _ecManager; // allocate on stack
    };
}
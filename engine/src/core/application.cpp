#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "pch.h"
#include "core/input-system.h"
#include "core/memory.h"
#include "events/domain.h"
#include "application.h"
#include "ecs/scene_manager.h"
#include "resource/manager.h"
#include "window.h"
#include "graphics/device.h"

namespace ge
{
    // singleton
    // internal link
    static AppConfig config;
    static bool _running{true};
    static ecs::SceneManager *sm{nullptr};
    static double dt, lastTick;

    // stack-based memory allocator
    // pre-allocated memory block
    // life-cyle is per-frame
    std::array<std::byte, 1024> buf;
    std::pmr::monotonic_buffer_resource singleFrameAllocator{buf.data(), buf.size(),
                                                             std::pmr::null_memory_resource()};

    // // life-cyle is entire app
    // std::array<std::byte, 1024> buf2;
    // std::pmr::monotonic_buffer_resource ge::appAllocator{buf2.data(), buf2.size(),
    //                                                  std::pmr::null_memory_resource()};

    GE_INLINE bool onQuit(const QuitEvent &)
    {
        return _running = false;
    }
    GE_INLINE void updateDt()
    {
        lastTick = glfwGetTime();
        dt = glfwGetTime() - lastTick;
    }
    void runEventLoop(const AppConfig &cfg)
    {
        config = cfg;
        // window
        Window window;
        window.init(&config.winCfg);

        // input
        InputSystem::instance()->init(nullptr);
        RegisterInputCallback();
        auto ed = ge::InputSystem::instance()->eventDispatcher();
        ed->subscribe<QuitEvent>(onQuit);

        // memory
        MemoryManager::instance()->init(nullptr);
        auto *heapAllocator = &MemoryManager::instance()->heapAllocator();
        auto *stackAllocator = &MemoryManager::instance()->stackAllocator();

        // graphics
        const auto &[width, height, title] = config.winCfg;
        GPUDeviceCreateInfo ci;
        ci.setWindow(width, height, window.nativeHandle()).setCustomAllocator(heapAllocator).setStackAllocator(stackAllocator);

        void *const p = appAllocator.allocate(sizeof(ecs::SceneManager), alignof(ecs::SceneManager));
        sm = new (p) ecs::SceneManager();
        sm->start();

        // lastTick = glfwGetTime();
        while (_running)
        {
            // // singleFrameAllocator.release();
            // updateDt();
            // sm->update(dt);
            window.pollEvents();

            // collect all the event and deal with them all at once
            InputSystem::instance()->dispatchEvents();
        }
        InputSystem::instance()->shutdown();
        window.shutdown();
        exit(EXIT_SUCCESS);
    }
}
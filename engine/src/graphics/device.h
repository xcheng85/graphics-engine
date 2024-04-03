#pragma once
#include "pch.h"
#if (_MSC_VER)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define VK_USE_PLATFORM_WIN32_KHR
#else
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>
namespace ge
{
    class IMemoryAllocator;
    class StackAllocator;

    struct GPUDeviceCreateInfo
    {
        void *window = nullptr; // OS window
        uint16_t width{1}, height{1};
        // for long-lived lifecycle
        IMemoryAllocator *customAllocator{nullptr};
        // for temp-lived lifecycle
        StackAllocator *stackAllocator{nullptr};
        // builder pattern to chain
        GPUDeviceCreateInfo &setWindow(uint16_t width, uint16_t height, void *handle);
        GPUDeviceCreateInfo &setCustomAllocator(IMemoryAllocator *allocator);
        GPUDeviceCreateInfo &setStackAllocator(StackAllocator *allocator);
    };

    class GPUDevice
    {
    public:
        void init(const GPUDeviceCreateInfo &creation) override;
        void shutdown() override;

    private:
        VkAllocationCallbacks *vulkan_allocation_callbacks{nullptr};
    };
}
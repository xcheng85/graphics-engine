#include "device.h"

namespace ge
{
    GPUDeviceCreateInfo &GPUDeviceCreateInfo::setWindow(uint16_t width, uint16_t height, void *handle)
    {
        width = width;
        height = height;
        window = handle;
        return *this;
    }

    GPUDeviceCreateInfo &GPUDeviceCreateInfo::setCustomAllocator(IMemoryAllocator *allocator)
    {
        customAllocator = allocator;
        return *this;
    }

    GPUDeviceCreateInfo &GPUDeviceCreateInfo::setStackAllocator(StackAllocator *allocator)
    {
        stackAllocator = allocator;
        return *this;
    }

    static constexpr char *requestedInstanceLayers[] = {
#if defined(VULKAN_DEBUG_REPORT)
        "VK_LAYER_KHRONOS_validation", // for validation
#else
        "",
#endif
    };

    static const char *requestedInstanceExtensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef VK_USE_PLATFORM_WIN32_KHR
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
        VK_MVK_MACOS_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XCB_KHR)
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
        VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XCB_KHR)
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
        VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_MIR_KHR || VK_USE_PLATFORM_DISPLAY_KHR)
        VK_KHR_DISPLAY_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
        VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_IOS_MVK)
        VK_MVK_IOS_SURFACE_EXTENSION_NAME,
#endif

#if defined(VULKAN_DEBUG_REPORT)
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, // enable fine-grained features
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
        VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME, // best practice validation
    };

    static VkBool32 vulkanDebugUtilsCB(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            std::cout << std::format("{}-{}: {}\n", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
        }
        else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            std::cerr << std::format("{}-{}: {}\n", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
        }
        return VK_FALSE;
    }

    void GPUDevice::init(const GPUDeviceCreateInfo &ci)
    {
        // // 1. Perform common code
        // allocator = creation.allocator;
        // temporary_allocator = creation.temporary_allocator;
        // string_buffer.init(1024 * 1024, creation.allocator);
        VkResult result;
        VkApplicationInfo appInfo =
            {VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "GE Vulkan Device", 1, "GE", 1, VK_MAKE_VERSION(1, 3, 280)};

        VkInstanceCreateInfo instanceCI = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            0,
            &appInfo,
#if defined(VULKAN_DEBUG_REPORT)
            GE_C_ARRAY_SIZE(requestedInstanceLayers),
            requestedInstanceLayers,
#else
            0,
            nullptr,
#endif
            GE_C_ARRAY_SIZE(requestedInstanceExtensions),
            requestedInstanceExtensions
        };

#if defined(VULKAN_DEBUG_REPORT)
        const VkDebugUtilsMessengerCreateInfoEXT debug_create_info{VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        creation_info.pfnUserCallback = vulkanDebugUtilsCB;
        creation_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        creation_info.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        create_info.pNext = &debug_create_info;
#endif
        result = vkCreateInstance(&create_info, vulkan_allocation_callbacks, &vulkan_instance);
        check(result);
    }
}
#define VK_NO_PROTOTYPES // for volk
#define VOLK_IMPLEMENTATION
#include "volk.h"
#include <window.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

#define ASSERT(expr, message) \
    {                         \
        void(message);        \
        assert(expr);         \
    }

#define VK_CHECK(func)                                                                   \
    {                                                                                    \
        const VkResult result = func;                                                    \
        if (result != VK_SUCCESS)                                                        \
        {                                                                                \
            std::cerr << "Error calling function " << #func << " at " << __FILE__ << ":" \
                      << __LINE__ << ". Result is " << result << std::endl;              \
            assert(false);                                                               \
        }                                                                                \
    }

bool gRunning = true;

Window gWindow;
VkInstance gInstance = VK_NULL_HANDLE;
VkDebugUtilsMessengerEXT gDebugUtilsMessenger = VK_NULL_HANDLE;
VkSurfaceKHR gVulkanWindowSurface = VK_NULL_HANDLE;
VkDevice gLogicDevice = VK_NULL_HANDLE;

static VkBool32 debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                       VkDebugUtilsMessageTypeFlagsEXT types,
                                       const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                       void *user_data)
{
    // cout << format(" MessageID: {} {}\nMessage: {}\n\n", callback_data->pMessageIdName, callback_data->messageIdNumber, callback_data->pMessage);

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        // __debugbreak();
    }

    return VK_FALSE;
}

void createVulkanInstance()
{
    // 1. check layers
    {
        // requested layer: "VK_LAYER_KHRONOS_validation"
        uint32_t instanceLayerCount{0};
        VK_CHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));
        std::vector<VkLayerProperties> layers(instanceLayerCount);
        VK_CHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, layers.data()));

        std::vector<std::string> layerNames;
        std::transform(
            layers.begin(), layers.end(), std::back_inserter(layerNames),
            [](const VkLayerProperties &properties)
            { return properties.layerName; });

        std::cout << "Found " << instanceLayerCount << " available layer(s)" << std::endl;
        for (const auto &layer : layerNames)
        {
            std::cout << "\t" << layer << std::endl;
        }
    }
    // 2. check instance extensions
    {
        // requested instance extensions

        //   VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        //   VK_KHR_SURFACE_EXTENSION_NAME,
        //   VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        //   VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        //   VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME,
        //  std::initializer_list<std::string> instanceExts = {
        // "VK_KHR_surface",
        // "VK_KHR_xcb_surface",
        // VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        // VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        // VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME};

        uint32_t extensionsCount{0};
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
        std::vector<VkExtensionProperties> extensionProperties(extensionsCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount,
                                               extensionProperties.data());

        std::vector<std::string> instanceExtensionNames;
        std::transform(extensionProperties.begin(), extensionProperties.end(),
                       std::back_inserter(instanceExtensionNames),
                       [](const VkExtensionProperties &properties)
                       { return properties.extensionName; });

        std::cout << "Found " << extensionsCount << " extension(s) for the instance" << std::endl;
        for (const auto &name : instanceExtensionNames)
        {
            std::cout << "\t" << name << std::endl;
        }

        // VK_EXT_debug_report and VK_EXT_debug_utils. Debug report was the first one and has essentially been abandoned in favor of the second
    }
    // single-linked-list
    // validationFeatures --> layer_settings_create_info --> messengerInfo --> nullptr

    // three extensions to the vk instance
    // 3. VkDebugUtilsMessengerCreateInfoEXT
    VkDebugUtilsMessengerCreateInfoEXT messengerInfo;
    {
        messengerInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .flags = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = &debugMessengerCallback,
            .pUserData = nullptr,
        };
    }

    // 4. VkLayerSettingsCreateInfoEXT - Specify layer capabilities for a Vulkan instance
    VkLayerSettingsCreateInfoEXT layer_settings_create_info;
    {
        const std::string layer_name = "VK_LAYER_KHRONOS_validation";
        const std::array<const char *, 1> setting_debug_action = {"VK_DBG_LAYER_ACTION_BREAK"};
        const std::array<const char *, 1> setting_gpu_based_action = {
            "GPU_BASED_DEBUG_PRINTF"};
        const std::array<VkBool32, 1> setting_printf_to_stdout = {VK_TRUE};
        const std::array<VkBool32, 1> setting_printf_verbose = {VK_TRUE};
        const std::array<VkLayerSettingEXT, 4> settings = {
            VkLayerSettingEXT{
                .pLayerName = layer_name.c_str(),
                .pSettingName = "debug_action",
                .type = VK_LAYER_SETTING_TYPE_STRING_EXT,
                .valueCount = 1,
                .pValues = setting_debug_action.data(),
            },
            VkLayerSettingEXT{
                .pLayerName = layer_name.c_str(),
                .pSettingName = "validate_gpu_based",
                .type = VK_LAYER_SETTING_TYPE_STRING_EXT,
                .valueCount = 1,
                .pValues = setting_gpu_based_action.data(),
            },
            VkLayerSettingEXT{
                .pLayerName = layer_name.c_str(),
                .pSettingName = "printf_to_stdout",
                .type = VK_LAYER_SETTING_TYPE_BOOL32_EXT,
                .valueCount = 1,
                .pValues = setting_printf_to_stdout.data(),
            },
            VkLayerSettingEXT{
                .pLayerName = layer_name.c_str(),
                .pSettingName = "printf_verbose",
                .type = VK_LAYER_SETTING_TYPE_BOOL32_EXT,
                .valueCount = 1,
                .pValues = setting_printf_verbose.data(),
            },
        };

        layer_settings_create_info = {
            .sType = VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT,
            .pNext = &messengerInfo,
            .settingCount = static_cast<uint32_t>(settings.size()),
            .pSettings = settings.data(),
        };
    }
    // 5. VkValidationFeaturesEXT - Specify validation features to enable or disable for a Vulkan instance
    VkValidationFeaturesEXT validationFeatures;
    {
        // VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT: specifies that the layers will process debugPrintfEXT operations in shaders and send the resulting output to the debug callback.
        // VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT: specifies that GPU-assisted validation is enabled. Activating this feature instruments shader programs to generate additional diagnostic data. This feature is disabled by default
        std::vector<VkValidationFeatureEnableEXT> validationFeaturesEnabled{
            VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT};
        // linked list
        validationFeatures = {
            .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
            .pNext = &layer_settings_create_info,
            .enabledValidationFeatureCount = static_cast<uint32_t>(validationFeaturesEnabled.size()),
            .pEnabledValidationFeatures = validationFeaturesEnabled.data(),
        };
    }
    // 6. VkApplicationInfo
    VkApplicationInfo applicationInfo;
    {
        applicationInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "TestVulkan",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_3,
        };
    }
    // 7. create vkInstance
    {
        {
            // SDL2 specific
            unsigned int extCount = 0;
            SDL_Vulkan_GetInstanceExtensions(gWindow.nativeHandle(), &extCount, nullptr);
            std::vector<const char *> exts(extCount);
            SDL_Vulkan_GetInstanceExtensions(gWindow.nativeHandle(), &extCount, exts.data());
            std::cout << "SDL2 Found " << extCount << " extension(s) for the instance" << std::endl;
            // VK_KHR_xlib_surface
            for (const auto &name : exts)
            {
                std::cout << "\t" << name << std::endl;
            }
        }

        // hardcodeded layers and extensions for now
        std::vector<const char *> instanceLayers{"VK_LAYER_KHRONOS_validation"};
        // debug report extension is deprecated
        // The window must have been created with the SDL_WINDOW_VULKAN flag and instance must have been created
        // with extensions returned by SDL_Vulkan_GetInstanceExtensions() enabled.
        std::vector<const char *> instanceExtensions{
            VK_KHR_SURFACE_EXTENSION_NAME,
            "VK_KHR_xlib_surface", // sdl2
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            // VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME,
        };

        const VkInstanceCreateInfo instanceInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = &validationFeatures,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = static_cast<uint32_t>(instanceLayers.size()),
            .ppEnabledLayerNames = instanceLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
            .ppEnabledExtensionNames = instanceExtensions.data(),
        };
        VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &gInstance));

        ASSERT(gInstance != VK_NULL_HANDLE, "Error creating VkInstance");

        // Initialize volk for this instance
        volkLoadInstance(gInstance);
    }
    // 8. Create Debug Utils Messenger
    {
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(gInstance, &messengerInfo, nullptr, &gDebugUtilsMessenger));
        ASSERT(gDebugUtilsMessenger != VK_NULL_HANDLE, "Error creating gDebugUtilsMessenger");
    }
    // 9. Surface for OS Window (surface properties used to select physical device)
    {
        // Caution:
        // The window must have been created with the SDL_WINDOW_VULKAN flag and instance must have been created
        // with extensions returned by SDL_Vulkan_GetInstanceExtensions() enabled.
        auto sdlWindow = gWindow.nativeHandle();
        SDL_Vulkan_CreateSurface(sdlWindow, gInstance, &gVulkanWindowSurface);
        ASSERT(gVulkanWindowSurface != VK_NULL_HANDLE, "Error creating gVulkanWindowSurface");
    }

    // 10. Select Physical Device based on surface
    // family queue of discrete gpu support the surface of native window
    VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
    uint32_t familyIndexSupportSurface = std::numeric_limits<u_int32_t>::max();
    {
        //  {VK_KHR_SWAPCHAIN_EXTENSION_NAME},  // physical device extensions
        uint32_t physicalDeviceCount{0};
        VK_CHECK(vkEnumeratePhysicalDevices(gInstance, &physicalDeviceCount, nullptr));
        ASSERT(physicalDeviceCount > 0, "No Vulkan Physical Devices found");
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(gInstance, &physicalDeviceCount, physicalDevices.data()));
        std::cout << "Found " << physicalDeviceCount << " Vulkan capable device(s)" << std::endl;

        // select physical gpu
        VkPhysicalDevice discrete_gpu = VK_NULL_HANDLE;
        VkPhysicalDevice integrated_gpu = VK_NULL_HANDLE;

        VkPhysicalDeviceProperties prop;
        for (u_int32_t i = 0; i < physicalDeviceCount; ++i)
        {
            VkPhysicalDevice physicalDevice = physicalDevices[i];
            vkGetPhysicalDeviceProperties(physicalDevice, &prop);

            if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

                std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

                VkBool32 surfaceSupported;
                for (uint32_t familyIndex = 0; familyIndex < queueFamilyCount; ++familyIndex)
                {
                    const VkQueueFamilyProperties &queueFamilyProp = queueFamilies[familyIndex];
                    // graphics or GPGPU family queue
                    if (queueFamilyProp.queueCount > 0 && (queueFamilyProp.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)))
                    {
                        vkGetPhysicalDeviceSurfaceSupportKHR(
                            physicalDevice,
                            familyIndex,
                            gVulkanWindowSurface,
                            &surfaceSupported);

                        if (surfaceSupported)
                        {
                            familyIndexSupportSurface = familyIndex;
                            discrete_gpu = physicalDevice;

                            break;
                        }
                    }
                }
                continue;
            }

            if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

                std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

                uint32_t familyIndexSupportSurface;
                VkBool32 surfaceSupported;
                for (uint32_t familyIndex = 0; familyIndex < queueFamilyCount; ++familyIndex)
                {
                    const VkQueueFamilyProperties &queueFamilyProp = queueFamilies[familyIndex];
                    // graphics or GPGPU family queue
                    if (queueFamilyProp.queueCount > 0 && (queueFamilyProp.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)))
                    {
                        vkGetPhysicalDeviceSurfaceSupportKHR(
                            physicalDevice,
                            familyIndex,
                            gVulkanWindowSurface,
                            &surfaceSupported);

                        if (surfaceSupported)
                        {
                            familyIndexSupportSurface = familyIndex;
                            integrated_gpu = physicalDevice;

                            break;
                        }
                    }
                }
                continue;
            }
        }

        selectedPhysicalDevice = discrete_gpu ? discrete_gpu : integrated_gpu;
        ASSERT(selectedPhysicalDevice, "No Vulkan Physical Devices found");
        ASSERT(familyIndexSupportSurface != std::numeric_limits<uint32_t>::max(), "No Queue Family Index supporting surface found");
    }

    // 11. Logging physical device
    {
        // Enable all features
        VkPhysicalDeviceFeatures2 features_ = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
        vkGetPhysicalDeviceFeatures2(selectedPhysicalDevice, &features_);

        // Properties V1
        VkPhysicalDeviceProperties physicalDevicesProp1;
        vkGetPhysicalDeviceProperties(selectedPhysicalDevice, &physicalDevicesProp1);
        // query device limits
        // timestampPeriod is the number of nanoseconds required for a timestamp query to be incremented by 1.
        // See Timestamp Queries.
        // auto gpu_timestamp_frequency = physicalDevicesProp1.limits.timestampPeriod / (1000 * 1000);
        // auto s_ubo_alignment = vulkan_physical_properties.limits.minUniformBufferOffsetAlignment;
        // auto s_ssbo_alignemnt = vulkan_physical_properties.limits.minStorageBufferOffsetAlignment;

        std::cout << format("GPU Used: {}, Vendor: {}, Device: {}, apiVersion:{}.{}.{}.{}",
                            physicalDevicesProp1.deviceName,
                            physicalDevicesProp1.vendorID,
                            physicalDevicesProp1.deviceID,
                            VK_API_VERSION_MAJOR(physicalDevicesProp1.apiVersion),
                            VK_API_VERSION_MINOR(physicalDevicesProp1.apiVersion),
                            VK_API_VERSION_PATCH(physicalDevicesProp1.apiVersion),
                            VK_API_VERSION_VARIANT(physicalDevicesProp1.apiVersion))
                  << endl;

        // If the VkPhysicalDeviceSubgroupProperties structure is included in the pNext chain of the VkPhysicalDeviceProperties2 structure passed to vkGetPhysicalDeviceProperties2,
        // it is filled in with each corresponding implementation-dependent property.
        // linked-list
        VkPhysicalDeviceSubgroupProperties subgroupProp{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES};
        subgroupProp.pNext = NULL;
        VkPhysicalDeviceProperties2 physicalDevicesProp{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
        physicalDevicesProp.pNext = &subgroupProp;
        vkGetPhysicalDeviceProperties2(selectedPhysicalDevice, &physicalDevicesProp);

        // Get memory properties
        VkPhysicalDeviceMemoryProperties2 memoryProperties_ = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
        };
        vkGetPhysicalDeviceMemoryProperties2(selectedPhysicalDevice, &memoryProperties_);

        // extensions for this selected device
        uint32_t extensionPropertyCount{0};
        VK_CHECK(vkEnumerateDeviceExtensionProperties(selectedPhysicalDevice, nullptr, &extensionPropertyCount, nullptr));
        std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(selectedPhysicalDevice, nullptr, &extensionPropertyCount, extensionProperties.data()));
        // convert to c++ string
        std::vector<std::string> extensions;
        std::transform(extensionProperties.begin(), extensionProperties.end(), std::back_inserter(extensions),
                       [](const VkExtensionProperties &property)
                       {
                           return std::string(property.extensionName);
                       });
        std::cout << "physical device extensions: " << endl;
        std::copy(std::begin(extensions), std::end(extensions), std::ostream_iterator<string>(cout, "\n"));
    }

    // 12. create logic device
    {
        // must use the extension supported by the selected physical device
        // hard coded here
        constexpr uint32_t deviceExtensionCount = 18;
        const char *deviceExtensions[] = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
            VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
            VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
            VK_NV_MESH_SHADER_EXTENSION_NAME,            // mesh_shaders_extension_present
            VK_KHR_MULTIVIEW_EXTENSION_NAME,             // multiview_extension_present
            VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME, // fragment_shading_rate_present
            VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
            VK_KHR_MAINTENANCE2_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, // ray_tracing_present
            VK_KHR_SPIRV_1_4_EXTENSION_NAME,
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
            VK_KHR_RAY_QUERY_EXTENSION_NAME, // ray query
            VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME,
            VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,

        };

        // queue for the logic device
        const float queuePriority[] = {1.0f};
        VkDeviceQueueCreateInfo queueInfo[1] = {};
        queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo[0].queueFamilyIndex = familyIndexSupportSurface;
        queueInfo[0].queueCount = 1;
        queueInfo[0].pQueuePriorities = queuePriority;

        // Enable all features through single linked list
        // physicalFeatures2 --> dynamicRenderingFeatures --> nullptr;
        VkPhysicalDeviceFeatures2 physicalFeatures2 = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
        vkGetPhysicalDeviceFeatures2(selectedPhysicalDevice, &physicalFeatures2);
        VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR};
        physicalFeatures2.pNext = &dynamicRenderingFeatures;

        VkDeviceCreateInfo logicDeviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        logicDeviceCreateInfo.queueCreateInfoCount = sizeof(queueInfo) / sizeof(queueInfo[0]);
        logicDeviceCreateInfo.pQueueCreateInfos = queueInfo;
        logicDeviceCreateInfo.enabledExtensionCount = deviceExtensionCount;
        logicDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
        logicDeviceCreateInfo.pNext = &physicalFeatures2;

        VK_CHECK(vkCreateDevice(selectedPhysicalDevice, &logicDeviceCreateInfo, nullptr, &gLogicDevice));
        ASSERT(gLogicDevice, "Failed to create logic device");
    }
}

int main(int argc, char **argv)
{

    WindowConfig cfg{
        1920,
        1080,
        "demo"s};

    gWindow.init(&cfg);

    VK_CHECK(volkInitialize());

    createVulkanInstance();

    while (gRunning)
    {
        gWindow.pollEvents();
    }

    gWindow.shutdown();

    return 0;
}
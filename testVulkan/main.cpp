#define VK_NO_PROTOTYPES // for volk
#define VOLK_IMPLEMENTATION
#include "volk.h"
#include <window.h>
#include <assert.h>
#include <vector>
#include <algorithm>

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
VkInstance gInstance = nullptr;

static VkBool32 debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                       VkDebugUtilsMessageTypeFlagsEXT types,
                                       const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                       void *user_data)
{
    cout << format(" MessageID: {} {}\nMessage: {}\n\n", callback_data->pMessageIdName, callback_data->messageIdNumber, callback_data->pMessage);

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
        // hardcodeded layers and extensions for now
        std::vector<const char*> instanceLayers{"VK_LAYER_KHRONOS_validation"};
        // debug report extension is deprecated
        std::vector<const char*> instanceExtensions{
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            // VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME,
        };

//   std::vector<const char*> instanceLayers(enabledLayers_.size());
//   std::transform(enabledLayers_.begin(), enabledLayers_.end(), instanceLayers.begin(),
//                  std::mem_fn(&std::string::c_str));

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
    }
}

int main(int argc, char **argv)
{
    VK_CHECK(volkInitialize());
    createVulkanInstance();
    WindowConfig cfg{
        1920,
        1080,
        "demo"s};

    Window window;

    window.init(&cfg);

    while (gRunning)
    {
        window.pollEvents();
    }

    window.shutdown();

    return 0;
}
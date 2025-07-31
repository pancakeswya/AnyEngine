#include "render/apis/vk/instance.h"

#include "render/apis/vk/error.h"

#ifndef ANY_RELEASE
#include <cstring>
#include <utility>
#endif

namespace vk {

namespace {

#ifndef ANY_RELEASE

VKAPI_ATTR VkBool32 VKAPI_CALL MessageCallback(
  [[maybe_unused]]VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  [[maybe_unused]]VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void* user_data) {
  if (const auto log_callback = reinterpret_cast<LogCallbackType>(user_data); log_callback != nullptr) {
    log_callback("Vulkan Debug message: %s\n", callback_data->pMessage);
  }
  return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT GetMessengerCreateInfo() {
  return {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = MessageCallback
  };
}

bool LayersAreSupported() {
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char* layer : Instance::kLayers) {
    bool layer_found = false;
    for (const VkLayerProperties& layer_properties : available_layers) {
      if (std::strcmp(layer, layer_properties.layerName) == 0) {
        layer_found = true;
        break;
      }
    }
    if (!layer_found) {
      return false;
    }
  }
  return true;
}
#endif // ANY_RELEASE

VkInstance CreateInstance(
  char const* const* window_extensions,
  size_t window_extension_count
  #ifndef ANY_RELEASE
  , LogCallbackType log_callback
  #endif
) {
  constexpr VkApplicationInfo app_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "VulkanFun",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "Simple Engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = Instance::kApiVersion
  };
#ifndef ANY_RELEASE
  if (!LayersAreSupported()) {
    throw Error("Instance layers are not supported");
  }
  VkDebugUtilsMessengerCreateInfoEXT messenger_create_info = GetMessengerCreateInfo();
  messenger_create_info.pUserData = reinterpret_cast<void*>(log_callback);
#endif
  std::vector extensions(
    Instance::kExtensions.begin(),
    Instance::kExtensions.end()
  );
  extensions.insert(
    extensions.end(),
    window_extensions,
    window_extensions + window_extension_count
  );
  const VkInstanceCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#ifndef ANY_RELEASE
    .pNext = &messenger_create_info,
#endif
    .pApplicationInfo = &app_info,
#ifndef ANY_RELEASE
    .enabledLayerCount = static_cast<uint32_t>(Instance::kLayers.size()),
    .ppEnabledLayerNames = Instance::kLayers.data(),
#endif
    .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
    .ppEnabledExtensionNames = extensions.data(),
#ifdef USE_PORTABILITY
    .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR
#endif
  };
  VkInstance instance = VK_NULL_HANDLE;
  if (const VkResult result = vkCreateInstance(&create_info, nullptr, &instance); result != VK_SUCCESS) {
    throw Error("failed to create instance").WithCode(result);
  }
  return instance;
}

#ifndef ANY_RELEASE
#define vkGetInstanceProcAddrByType(instance, proc) reinterpret_cast<decltype(&(proc))>(vkGetInstanceProcAddr(instance, #proc))

inline VkDebugUtilsMessengerEXT CreateMessenger(VkInstance instance) {
  const auto create_messenger = vkGetInstanceProcAddrByType(instance, vkCreateDebugUtilsMessengerEXT);
  if (create_messenger == nullptr) {
    throw Error("Couldn't find vkCreateDebugUtilsMessengerEXT by procc addr");
  }
  const VkDebugUtilsMessengerCreateInfoEXT create_info = GetMessengerCreateInfo();
  VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
  if (const VkResult result = create_messenger(instance, &create_info, nullptr, &messenger); result != VK_SUCCESS) {
    throw Error("failed to set up debug messenger").WithCode(result);
  }
  return messenger;
}

inline auto GetMessengerDestroyFunc(VkInstance instance) {
  if (instance == VK_NULL_HANDLE) {
    throw Error("Inactive instance handle");
  }
  const auto destroy_messenger = vkGetInstanceProcAddrByType(instance, vkDestroyDebugUtilsMessengerEXT);
  if (destroy_messenger == nullptr) {
    throw Error("Couldn't find vkDestroyDebugUtilsMessengerEXT");
  }
  return destroy_messenger;
}
#endif // ANY_RELEASE

} // namespace

#ifndef ANY_RELEASE
DebugMessenger::DebugMessenger(VkInstance instance)
  : NonDispatchableRuntimeHandle(CreateMessenger(instance), instance, GetMessengerDestroyFunc(instance)) {}
#endif // ANY_RELEASE

Instance::Instance(
  char const* const* window_extensions,
  size_t window_extension_count
#ifndef ANY_RELEASE
  , LogCallbackType log_callback
#endif
) : Handle(CreateInstance(
    window_extensions,
    window_extension_count
#ifndef ANY_RELEASE
    , log_callback
#endif
  )) {}

std::vector<VkPhysicalDevice> Instance::devices() const {
  uint32_t device_count = 0;
  if (const VkResult result = vkEnumeratePhysicalDevices(handle_, &device_count, nullptr); result != VK_SUCCESS) {
    throw Error("failed to get physical devices count").WithCode(result);
  }
  if (device_count == 0) {
    throw Error("failed to find GPUs with Vulkan support");
  }
  std::vector<VkPhysicalDevice> devices(device_count);
  if (const VkResult result = vkEnumeratePhysicalDevices(handle_, &device_count, devices.data()); result != VK_SUCCESS) {
    throw Error("failed to get physical devices").WithCode(result);
  }
  return devices;
}

} // namespace vk
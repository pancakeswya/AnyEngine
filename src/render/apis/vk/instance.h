#ifndef RENDER_VK_INSTANCE_H_
#define RENDER_VK_INSTANCE_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

#include <array>

namespace vk {

#ifndef NDEBUG
struct DebugMessenger : NonDispatchableRuntimeHandle<VkDebugUtilsMessengerEXT, VkInstance> {
  DECLARE_DEFAULT_NO_COPY_CLASS(DebugMessenger);
  explicit DebugMessenger(VkInstance instance);
};
#endif // NDEBUG

struct Instance : Handle<VkInstance, vkDestroyInstance> {
  static constexpr std::array
#if defined(NDEBUG) && !defined(__APPLE__)
    <const char*, 0>
#endif
  kExtensions = {
#ifndef NDEBUG
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
#ifdef __APPLE__
    VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#endif // __APPLE__
  };
  static constexpr std::array
#if defined(NDEBUG)
  <const char*, 0>
#endif
  kLayers = {
#ifndef NDEBUG
    "VK_LAYER_KHRONOS_validation"
#endif
  };
  explicit Instance(const char* path);
  ~Instance();

  [[nodiscard]] std::vector<VkPhysicalDevice> devices() const;
};

} // namespace vk

#endif // RENDER_VK_INSTANCE_H_

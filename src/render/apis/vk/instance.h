#ifndef RENDER_VK_INSTANCE_H_
#define RENDER_VK_INSTANCE_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

#include <array>

namespace vk {

#ifndef ANY_RELEASE
struct DebugMessenger : NonDispatchableRuntimeHandle<VkDebugUtilsMessengerEXT, VkInstance> {
  DECLARE_DEFAULT_NO_COPY_CLASS(DebugMessenger);
  explicit DebugMessenger(VkInstance instance);
};
#endif // ANY_RELEASE

struct Instance : Handle<VkInstance, vkDestroyInstance> {
  static constexpr auto kApiVersion = VK_API_VERSION_1_0;

  static constexpr std::array
#if defined(ANY_RELEASE) && !defined(USE_PORTABILITY)
    <const char*, 0>
#endif
  kExtensions = {
#ifndef ANY_RELEASE
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
#ifdef USE_PORTABILITY
    VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#endif // USE_PORTABILITY
  };
  static constexpr std::array
#if defined(ANY_RELEASE)
  <const char*, 0>
#endif
  kLayers = {
#ifndef ANY_RELEASE
    "VK_LAYER_KHRONOS_validation"
#endif
  };
  explicit Instance(const char* path);
  ~Instance();

  [[nodiscard]] std::vector<VkPhysicalDevice> devices() const;
};

} // namespace vk

#endif // RENDER_VK_INSTANCE_H_

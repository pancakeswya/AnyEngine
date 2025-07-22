#ifndef RENDER_APIS_VK_DEVICE_H_
#define RENDER_APIS_VK_DEVICE_H_

#include "render/apis/vk/handle.h"
#include "render/apis/vk/error.h"
#include "render/apis/vk/queue.h"

#include <vulkan/vulkan.h>

#include <array>
#include <vector>

namespace vk {

struct QueueFamilyIndices {
  uint32_t graphics;
  uint32_t present;
};

struct Queues {
  DECLARE_DEFAULT_NO_COPY_CLASS(Queues);

  Queues(VkDevice device, const QueueFamilyIndices& queue_family_indices);

  Queue graphics;
  Queue present;
};

struct PhysicalDevice : IdentityHandle<VkPhysicalDevice> {
  DECLARE_DEFAULT_CLASS(PhysicalDevice);

  struct SurfaceSupport {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };

  struct Requirements {
    bool present;
    bool graphic;
    bool anisotropy;

    VkSurfaceKHR surface;

    std::vector<const char*> extensions;
    std::vector<const char*> layers;
  };

  explicit PhysicalDevice(VkPhysicalDevice physical_device) noexcept;

  [[nodiscard]] bool IsSuitable(const Requirements& requirements, QueueFamilyIndices& queue_family_indices) const;
  [[nodiscard]] SurfaceSupport GetSurfaceSupport(VkSurfaceKHR surface) const;
  [[nodiscard]] bool CheckFormatFeatureSupported(VkFormat format, VkFormatFeatureFlagBits feature) const noexcept;
  [[nodiscard]] uint32_t FindMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
  [[nodiscard]] VkFormat FindSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features) const;
  [[nodiscard]] bool CheckExtensionsSupport(const std::vector<const char*>& extensions) const;
  [[nodiscard]] std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties() const;
  [[nodiscard]] VkBool32 CheckSurfaceSupported(VkSurfaceKHR surface, uint32_t queue_family_idx) const;
  [[nodiscard]] VkPhysicalDeviceFeatures GetFeatures() const;
};

class Device : public Handle<VkDevice, vkDestroyDevice> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Device);

  static constexpr std::array kExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
  #ifdef USE_PORTABILITY
    VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
  #endif
  };

  static Device Select(
    const std::vector<VkPhysicalDevice>& physical_devices,
    const PhysicalDevice::Requirements& requirements
  );

  [[nodiscard]] const PhysicalDevice& physical_device() const noexcept {
    return physical_device_;
  }

  [[nodiscard]] const Queues& queues() const noexcept {
    return queues_;
  }
private:
  Device(
     const PhysicalDevice& physical_device,
     const PhysicalDevice::Requirements& requirements,
     const QueueFamilyIndices& queue_family_indices
  );

  PhysicalDevice physical_device_;
  Queues queues_;
};

struct DeviceMemory : NonDispatchableHandle<VkDeviceMemory, VkDevice, vkFreeMemory> {
  DECLARE_DEFAULT_NO_COPY_CLASS(DeviceMemory);

  DeviceMemory(
    const Device& device,
    VkMemoryPropertyFlags properties,
    const VkMemoryRequirements& requirements
  );

  template<typename T>
  T* Map(const VkDeviceSize size = VK_WHOLE_SIZE) const {
    void* data;
    if (const VkResult result = vkMapMemory(parent_, handle_, 0, size, 0, &data); result != VK_SUCCESS) {
      throw Error("failed to map buffer memory").WithCode(result);
    }
    return static_cast<T*>(data);
  }

  void Unmap() const noexcept {
    vkUnmapMemory(parent_, handle_);
  }
};

} // namespace vk

#endif // RENDER_APIS_VK_DEVICE_H_
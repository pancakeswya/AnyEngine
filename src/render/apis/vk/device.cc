#include "render/apis/vk/device.h"

#include <set>
#include <optional>

namespace vk {

namespace {

VkDevice CreateDevice(
  VkPhysicalDevice physical_device,
  const std::vector<const char*>& extensions,
  const std::vector<const char*>& layers,
  VkBool32 anisotropy,
  const QueueFamilyIndices& indices
) {
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set unique_family_ids = {
    indices.graphics,
    indices.present
  };
  constexpr float queue_priority = 1.0f;
  for(const unsigned int family_idx : unique_family_ids) {
    const VkDeviceQueueCreateInfo queue_create_info = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = family_idx,
      .queueCount = 1,
      .pQueuePriorities = &queue_priority
    };
    queue_create_infos.push_back(queue_create_info);
  }
  const VkPhysicalDeviceFeatures device_features = {
    .samplerAnisotropy = anisotropy
  };
  VkDeviceCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
    .pQueueCreateInfos = queue_create_infos.data(),
    .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
    .ppEnabledExtensionNames = extensions.data(),
    .pEnabledFeatures = &device_features,
  };
  if (!layers.empty()) {
    create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
    create_info.ppEnabledLayerNames = layers.data();
  } else {
    create_info.enabledLayerCount = 0;
  }
  VkDevice logical_device = VK_NULL_HANDLE;
  if (const VkResult result = vkCreateDevice(physical_device, &create_info, nullptr, &logical_device); result != VK_SUCCESS) {
    throw Error("failed to create device").WithCode(result);
  }
  return logical_device;
}

VkDeviceMemory CreateMemory(
  const Device& device,
  const VkMemoryPropertyFlags properties,
  const VkMemoryRequirements& requirements
) {
  const VkMemoryAllocateInfo info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .allocationSize = requirements.size,
    .memoryTypeIndex = device.physical_device().FindMemoryType(
      requirements.memoryTypeBits,
      properties
    )
  };
  VkDeviceMemory memory;
  if (const VkResult result = vkAllocateMemory(device, &info, nullptr, &memory); result != VK_SUCCESS) {
    throw Error("failed to allocate memory").WithCode(result);
  }
  return memory;
}

} // namespace

Queues::Queues(VkDevice device, const QueueFamilyIndices& queue_family_indices) :
  graphics(device, queue_family_indices.graphics),
  present(device, queue_family_indices.present) {}

bool PhysicalDevice::IsSuitable(const Requirements& requirements, QueueFamilyIndices& queue_family_indices) const {
  std::optional<uint32_t> graphic, present;

  std::vector<VkQueueFamilyProperties> queue_family_props = GetQueueFamilyProperties();

  for (size_t i = 0; i < queue_family_props.size(); ++i) {
    if (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      graphic = static_cast<uint32_t>(i);
    }
    if (CheckSurfaceSupported(requirements.surface, i)) {
      present = static_cast<uint32_t>(i);
    }
    if ((requirements.graphic && !graphic.has_value()) ||
        (requirements.present && !present.has_value())) {
      continue;
    }
    if (const VkPhysicalDeviceFeatures device_features = GetFeatures();
        (requirements.anisotropy && !device_features.samplerAnisotropy) ||
        !CheckExtensionsSupport(requirements.extensions)) {
      continue;
    }
    if (const SurfaceSupport support = GetSurfaceSupport(requirements.surface);
        !support.formats.empty() &&
        !support.present_modes.empty()) {
      queue_family_indices = {
        .graphics = graphic.value(),
        .present = present.value()
      };
      return true;
    }
  }
  return false;
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device) noexcept : IdentityHandle(physical_device) {}

PhysicalDevice::SurfaceSupport PhysicalDevice::GetSurfaceSupport(VkSurfaceKHR surface) const {
  SurfaceSupport support;
  if (const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle_, surface, &support.capabilities); result != VK_SUCCESS) {
    throw Error("failed to get physical device surface capabilities").WithCode(result);
  }
  uint32_t format_count;
  if (const VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle_, surface, &format_count, nullptr); result != VK_SUCCESS) {
    throw Error("failed to get physical device surface formats count").WithCode(result);
  }
  if (format_count != 0) {
    support.formats.resize(format_count);
    if (const VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle_, surface, &format_count, support.formats.data()); result != VK_SUCCESS) {
      throw Error("failed to get physical device surface formats").WithCode(result);
    }
  }
  uint32_t present_mode_count;
  if (const VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle_, surface, &present_mode_count, nullptr); result != VK_SUCCESS) {
    throw Error("failed to get physical device surface present modes count").WithCode(result);
  }

  if (present_mode_count != 0) {
    support.present_modes.resize(present_mode_count);
    if (const VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle_, surface, &present_mode_count, support.present_modes.data()); result != VK_SUCCESS) {
      throw Error("failed to get physical device surface present modes").WithCode(result);
    }
  }
  return support;
}

bool PhysicalDevice::CheckFormatFeatureSupported(const VkFormat format, const VkFormatFeatureFlagBits feature) const noexcept {
  VkFormatProperties format_properties = {};
  vkGetPhysicalDeviceFormatProperties(handle_, format, &format_properties);
  return (format_properties.optimalTilingFeatures & feature) != 0;
}

uint32_t PhysicalDevice::FindMemoryType(const uint32_t type_filter, const VkMemoryPropertyFlags properties) const {
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(handle_, &mem_properties);

  for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i) {
    if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }
  throw Error("failed to find suitable memory type!");
}

VkFormat PhysicalDevice::FindSupportedFormat(const std::vector<VkFormat>& formats, const VkImageTiling tiling, const VkFormatFeatureFlags features) const {
  for (const VkFormat format : formats) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(handle_, format, &props);
    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      return format;
    }
    if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  throw Error("failed to find supported format");
}

bool PhysicalDevice::CheckExtensionsSupport(const std::vector<const char*>& extensions) const {
  uint32_t extension_count;
  if (const VkResult result = vkEnumerateDeviceExtensionProperties(handle_, nullptr, &extension_count, nullptr); result != VK_SUCCESS) {
    throw Error("failed to get device extension properties count").WithCode(result);
  }
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  if (const VkResult result = vkEnumerateDeviceExtensionProperties(handle_, nullptr, &extension_count, available_extensions.data()); result != VK_SUCCESS) {
    throw Error("failed to get device extension properties").WithCode(result);
  }
  std::set required_extensions(extensions.begin(), extensions.end());

  for (const VkExtensionProperties& extension : available_extensions) {
    required_extensions.erase(extension.extensionName);
  }
  return required_extensions.empty();
}

std::vector<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties() const {
  uint32_t families_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(handle_, &families_count, nullptr);

  std::vector<VkQueueFamilyProperties> family_properties(families_count);
  vkGetPhysicalDeviceQueueFamilyProperties(handle_, &families_count, family_properties.data());

  return family_properties;
}

VkBool32 PhysicalDevice::CheckSurfaceSupported(VkSurfaceKHR surface, const uint32_t queue_family_idx) const {
  VkBool32 present_support = false;
  if (const VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(handle_, queue_family_idx, surface, &present_support); result != VK_SUCCESS) {
    throw Error("failed to get physical device surface support").WithCode(result);
  }
  return present_support;
}

VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures() const {
  VkPhysicalDeviceFeatures device_features;
  vkGetPhysicalDeviceFeatures(handle_, &device_features);

  return device_features;
}

Device Device::Select(const std::vector<VkPhysicalDevice>& physical_devices, const PhysicalDevice::Requirements& requirements) {
  for(VkPhysicalDevice physical_device : physical_devices) {
    PhysicalDevice device(physical_device);
    QueueFamilyIndices queue_family_indices = {};
    if (device.IsSuitable(requirements, queue_family_indices)) {
      return {device, requirements, queue_family_indices};
    }
  }
  throw Error("failed to find suitable device");
}

Device::Device(const PhysicalDevice& physical_device,
             const PhysicalDevice::Requirements& requirements,
             const QueueFamilyIndices& queue_family_indices)
: Handle(CreateDevice(physical_device, requirements.extensions, requirements.layers, requirements.anisotropy, queue_family_indices)),
  physical_device_(physical_device),
  queues_(handle_, queue_family_indices) {}

DeviceMemory::DeviceMemory(
 const Device& device,
 const VkMemoryPropertyFlags properties,
 const VkMemoryRequirements& requirements
) : NonDispatchableHandle(CreateMemory(device, properties, requirements), device) {}

} // namespace vk
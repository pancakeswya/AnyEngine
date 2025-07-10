#include "render/apis/vk/sampler.h"

#include "render/apis/vk/error.h"

#include <algorithm>

namespace vk {

namespace {

inline VkSampler CreateSampler(
  VkDevice device,
  VkPhysicalDevice physical_device,
  const VkSamplerMipmapMode mipmap_mode,
  const uint32_t mip_levels
) {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physical_device, &properties);

  const VkSamplerCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    .magFilter = VK_FILTER_LINEAR,
    .minFilter = VK_FILTER_LINEAR,
    .mipmapMode = mipmap_mode,
    .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .mipLodBias = 0.0f,
    .anisotropyEnable = VK_TRUE,
    .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
    .compareEnable = VK_FALSE,
    .compareOp = VK_COMPARE_OP_ALWAYS,
    .minLod = 0.0f,
    .maxLod = static_cast<float>(mip_levels),
    .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    .unnormalizedCoordinates = VK_FALSE
  };
  VkSampler sampler;
  if (const VkResult result = vkCreateSampler(device, &info, nullptr, &sampler); result != VK_SUCCESS) {
    throw Error("failed to create sampler").WithCode(result);
  }
  return sampler;
}

} // namespace

Sampler::Sampler(
  const Device& device,
  const VkSamplerMipmapMode mipmap_mode,
  const uint32_t mip_levels
) : NonDispatchableHandle(CreateSampler(device, device.physical_device(), mipmap_mode, mip_levels), device) {}

} // namespace vk
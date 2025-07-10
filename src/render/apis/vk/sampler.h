#ifndef RENDER_APIS_VK_SAMPLER_H_
#define RENDER_APIS_VK_SAMPLER_H_

#include "render/apis/vk/device.h"
#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

namespace vk {

struct Sampler : NonDispatchableHandle<VkSampler, VkDevice, vkDestroySampler> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Sampler);
  Sampler(
    const Device& device,
    VkSamplerMipmapMode mipmap_mode,
    uint32_t mip_levels
  );
};

} // namespace vk

#endif // RENDER_APIS_VK_SAMPLER_H_

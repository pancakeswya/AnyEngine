#ifndef RENDER_APIS_VK_FRAMEBUFFER_H_
#define RENDER_APIS_VK_FRAMEBUFFER_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

struct Framebuffer : NonDispatchableHandle<VkFramebuffer, VkDevice, vkDestroyFramebuffer> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Framebuffer);
  Framebuffer(
    VkDevice device,
    const std::vector<VkImageView>& views,
    VkRenderPass render_pass,
    VkExtent2D extent
  );
};

} // namespace vk

#endif // RENDER_APIS_VK_FRAMEBUFFER_H_

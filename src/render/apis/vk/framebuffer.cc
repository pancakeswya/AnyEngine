#include "render/apis/vk/framebuffer.h"

#include "render/apis/vk/error.h"

#include <algorithm>

namespace vk {

namespace {

inline VkFramebuffer CreateFramebuffer(
  VkDevice device,
  const std::vector<VkImageView>& views,
  VkRenderPass render_pass,
  const VkExtent2D extent
) {
  const VkFramebufferCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .renderPass = render_pass,
    .attachmentCount = static_cast<uint32_t>(views.size()),
    .pAttachments = views.data(),
    .width = extent.width,
    .height = extent.height,
    .layers = 1
  };
  VkFramebuffer framebuffer;
  if (const VkResult result = vkCreateFramebuffer(device, &info, nullptr, &framebuffer); result != VK_SUCCESS) {
    throw Error("failed to create framebuffer").WithCode(result);
  }
  return framebuffer;
}

} // namespace

Framebuffer::Framebuffer(
  VkDevice device,
  const std::vector<VkImageView>& views,
  VkRenderPass render_pass,
  const VkExtent2D extent
) : NonDispatchableHandle(CreateFramebuffer(device, views, render_pass, extent), device) {}

} // namespace vk
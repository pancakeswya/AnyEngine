#ifndef RENDER_APIS_VK_SWAPCHAIN_H_
#define RENDER_APIS_VK_SWAPCHAIN_H_

#include "render/apis/vk/device.h"
#include "render/apis/vk/framebuffer.h"
#include "render/apis/vk/handle.h"
#include "render/apis/vk/image.h"
#include "render/apis/vk/surface.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

class Swapchain final : public NonDispatchableHandle<VkSwapchainKHR, VkDevice, vkDestroySwapchainKHR> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Swapchain);
  Swapchain(
    const Device& device,
    const Surface& surface,
    VkExtent2D extent,
    VkSwapchainKHR old_swapchain = VK_NULL_HANDLE
  );

  [[nodiscard]] std::vector<VkImage> images() const;

  [[nodiscard]] const ImageInfo& image_info() const noexcept {
    return image_info_;
  }

  [[nodiscard]] uint32_t min_image_count() const noexcept {
    return min_image_count_;
  }
private:
  ImageInfo image_info_;
  uint32_t min_image_count_;
};

class SwapchainImagePresent {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(SwapchainImagePresent);
  SwapchainImagePresent(
    VkDevice device,
    VkRenderPass render_pass,
    VkImage swapchain_image,
    VkImageView depth_image_view,
    const ImageInfo& swapchain_image_info
  );

  [[nodiscard]] const ImageView& view() const noexcept {
    return view_;
  }

  [[nodiscard]] const Framebuffer& framebuffer() const noexcept {
    return framebuffer_;
  }
private:
  ImageView view_;
  Framebuffer framebuffer_;
};

} // namespace vk

#endif // RENDER_APIS_VK_SWAPCHAIN_H_

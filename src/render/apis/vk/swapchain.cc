#include "render/apis/vk/swapchain.h"

#include "render/apis/vk/error.h"

#include <algorithm>
#include <utility>
#include <limits>

namespace vk {

namespace {

inline VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) {
  for (const VkSurfaceFormatKHR& available_format : available_formats) {
    if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return available_format;
        }
  }
  return available_formats[0];
}

inline VkPresentModeKHR ChoosePresentMode([[maybe_unused]]const std::vector<VkPresentModeKHR>& available_present_modes) {
  //  for (const auto& available_present_mode : available_present_modes) {
  //    if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
  //      return available_present_mode;
  //    }
  //  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

inline VkExtent2D ChooseExtent(const VkExtent2D extent, const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }
  return {
    std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
    std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
  };
}

VkSwapchainKHR CreateSwapchain(
  const Device& device,
  VkSurfaceKHR surface,
  const VkExtent2D size,
  ImageInfo& info
) {
  const auto [capabilities, formats, present_modes] = device.physical_device().GetSurfaceSupport(surface);
  const auto[format, color_space] = ChooseSurfaceFormat(formats);
  const VkPresentModeKHR present_mode = ChoosePresentMode(present_modes);
  const VkExtent2D extent = ChooseExtent(size, capabilities);

  uint32_t image_count = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount) {
    image_count = capabilities.maxImageCount;
  }
  VkSwapchainCreateInfoKHR create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .surface = surface,
    .minImageCount = image_count,
    .imageFormat = format,
    .imageColorSpace = color_space,
    .imageExtent = extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
  };
  const std::array queue_family_indices = {
    device.queues().graphics.family_index(),
    device.queues().present.family_index()
  };
  if (queue_family_indices[0] != queue_family_indices[1]) {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = queue_family_indices.size();
    create_info.pQueueFamilyIndices = queue_family_indices.data();
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
  create_info.preTransform = capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  if ((capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) == 0) {
    if ((capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR) != 0)
      create_info.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
  }
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;

  VkSwapchainKHR swapchain;
  if (const VkResult result = vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain); result != VK_SUCCESS) {
    throw Error("failed to create swapchain").WithCode(result);
  }
  info.extent = extent;
  info.format = format;

  return swapchain;
}

} // namespace

Swapchain::Swapchain(
  const Device& device,
  const Surface& surface,
  const VkExtent2D extent
) : NonDispatchableHandle(CreateSwapchain(device,surface, extent, image_info_), device) {}

std::vector<VkImage> Swapchain::images() const {
  uint32_t image_count;
  if (const VkResult result = vkGetSwapchainImagesKHR(parent_, handle_, &image_count, nullptr); result != VK_SUCCESS) {
    throw Error("failed to get swapchain image count").WithCode(result);
  }
  std::vector<VkImage> images(image_count);
  if (const VkResult result = vkGetSwapchainImagesKHR(parent_, handle_, &image_count, images.data()); result != VK_SUCCESS) {
    throw Error("failed to get swapchain images").WithCode(result);
  }
  return images;
}

SwapchainFramebuffer::SwapchainFramebuffer(
  VkDevice device,
  VkRenderPass render_pass,
  VkImage swapchain_image,
  VkImageView depth_image_view,
  const ImageInfo& swapchain_image_info
) : view_(device, swapchain_image, VK_IMAGE_ASPECT_COLOR_BIT, swapchain_image_info.format),
    framebuffer_(device, { view_, depth_image_view }, render_pass, swapchain_image_info.extent) {}

} // namespace vk
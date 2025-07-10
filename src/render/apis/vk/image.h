#ifndef RENDER_APIS_VK_IMAGE_H_
#define RENDER_APIS_VK_IMAGE_H_

#include "render/apis/vk/device.h"
#include "render/apis/vk/command.h"
#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

namespace vk {

class Buffer;

struct ImageInfo {
  VkExtent2D extent;
  VkFormat format;
};

struct ImageMemory : DeviceMemory {
  DECLARE_DEFAULT_NO_COPY_CLASS(ImageMemory);
  ImageMemory(
    const Device& device,
    VkImage image,
    VkMemoryPropertyFlags properties
  );
};

struct ImageView : NonDispatchableHandle<VkImageView, VkDevice, vkDestroyImageView> {
  DECLARE_DEFAULT_NO_COPY_CLASS(ImageView);
  ImageView(
    VkDevice device,
    VkImage image,
    VkImageAspectFlags aspect_flags,
    VkFormat format,
    uint32_t mip_levels = 1
  );
};

class Image : public NonDispatchableHandle<VkImage, VkDevice, vkDestroyImage> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Image);
  Image(
    const Device& device,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkImageAspectFlags aspect_flags,
    VkExtent2D extent,
    VkFormat format,
    VkImageTiling tiling,
    uint32_t mip_levels = 1
  );

  [[nodiscard]] const ImageView& view() const noexcept {
    return view_;
  }

  [[nodiscard]] const ImageMemory& memory() const noexcept {
    return memory_;
  }

  [[nodiscard]] const ImageInfo& info() const noexcept {
    return info_;
  }

  [[nodiscard]] uint32_t mip_levels() const noexcept {
    return mip_levels_;
  }
private:
  ImageMemory memory_;
  ImageView view_;
  ImageInfo info_ = {};

  uint32_t mip_levels_ = 0;
};

class ImageCommander : public Commander {
public:
  ImageCommander(Image& image, const CommandPool& command_pool, VkQueue graphics_queue);
  ~ImageCommander() = default;

  void GenerateMipmaps() const;
  void TransitImageLayout(VkImageLayout old_layout, VkImageLayout new_layout) const;
  void CopyBuffer(const Buffer& src) const;
private:
  Image& image_;
};

struct StagingImage : Image {
  DECLARE_DEFAULT_NO_COPY_CLASS(StagingImage);
  StagingImage(
    const Device& device,
    VkExtent2D extent,
    VkFormat format
  );
};

} // namespace vk

#endif // RENDER_APIS_VK_IMAGE_H_

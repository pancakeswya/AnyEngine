#include "render/apis/vk/image.h"

#include "render/apis/vk/command.h"
#include "render/apis/vk/error.h"

#include <utility>
#include <cmath>

namespace vk {

namespace {

inline uint32_t CalculateMipMaps(const VkExtent2D extent) {
  return static_cast<uint32_t>(std::floor(std::log2(std::max(extent.width, extent.height)))) + 1;
}

inline VkImageView CreateImageView(
  VkDevice device,
  VkImage image,
  const VkImageAspectFlags aspect_flags,
  const VkFormat format,
  const uint32_t mip_levels
) {
  const VkImageViewCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .image = image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = format,
    .subresourceRange = {
      .aspectMask = aspect_flags,
      .baseMipLevel = 0,
      .levelCount = mip_levels,
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };
  VkImageView view = VK_NULL_HANDLE;
  if (const VkResult result = vkCreateImageView(device, &info, nullptr, &view); result != VK_SUCCESS) {
    throw Error("failed to create image view").WithCode(result);
  }
  return view;
}

inline VkImage CreateImage(
  VkDevice device,
  const VkImageUsageFlags usage,
  const VkExtent2D extent,
  const VkFormat format,
  const VkImageTiling tiling,
  const uint32_t mip_levels
) {
  const VkImageCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .imageType = VK_IMAGE_TYPE_2D,
    .format = format,
    .extent = { extent.width, extent.height, 1 },
    .mipLevels = mip_levels,
    .arrayLayers = 1,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .tiling = tiling,
    .usage = usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
  };
  VkImage image;
  if (const VkResult result = vkCreateImage(device, &info, nullptr, &image); result != VK_SUCCESS) {
    throw Error("failed to create image").WithCode(result);
  }
  return image;
}

inline VkMemoryRequirements GetMemoryRequirements(VkDevice device, VkImage image) {
  VkMemoryRequirements requirements;
  vkGetImageMemoryRequirements(device, image, &requirements);
  return requirements;
}

Image CreateStagingImage(
  const Device& device,
  const VkExtent2D extent,
  const VkFormat format
) {
  if (!device.physical_device().CheckFormatFeatureSupported(
          format, VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    throw Error("image format does not support linear blitting");
  }

  constexpr VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                      VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                      VK_IMAGE_USAGE_SAMPLED_BIT;
  constexpr VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

  Image image(device,
    usage,
    properties,
    VK_IMAGE_ASPECT_COLOR_BIT,
    extent,
    format,
    VK_IMAGE_TILING_OPTIMAL,
    CalculateMipMaps(extent)
  );
  return image;
}

} // namespace

ImageMemory::ImageMemory(
  const Device& device,
  VkImage image,
  const VkMemoryPropertyFlags properties
) : DeviceMemory(device, properties, GetMemoryRequirements(device, image)) {
  if (const VkResult result = vkBindImageMemory(device, image, *this, 0); result != VK_SUCCESS) {
    throw Error("failed to bind image memory").WithCode(result);
  }
}

ImageView::ImageView(
   VkDevice device,
   VkImage image,
   const VkImageAspectFlags aspect_flags,
   const VkFormat format,
   const uint32_t mip_levels)
  : NonDispatchableHandle(CreateImageView(device, image, aspect_flags, format, mip_levels), device) {}

Image::Image(
    const Device& device,
    const VkImageUsageFlags usage,
    const VkMemoryPropertyFlags properties,
    const VkImageAspectFlags aspect_flags,
    const VkExtent2D extent,
    const VkFormat format,
    const VkImageTiling tiling,
    const uint32_t mip_levels
) : NonDispatchableHandle(CreateImage(device, usage, extent, format, tiling, mip_levels), device),
    memory_(device, handle_, properties),
    view_(device, handle_, aspect_flags, format, mip_levels),
    info_{.extent = extent, .format = format},
    mip_levels_(mip_levels) {}

ImageCommander::ImageCommander(Image& image, const CommandPool& cmd_pool, VkQueue graphics_queue)
    : Commander(cmd_pool, graphics_queue),
      image_(image) {}

void ImageCommander::GenerateMipmaps() const {
  VkImageMemoryBarrier barrier = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = image_,
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };
  auto mip_width = static_cast<int32_t>(image_.info().extent.width);
  auto mip_height = static_cast<int32_t>(image_.info().extent.height);

  for (uint32_t i = 1; i < image_.mip_levels(); i++) {
      barrier.subresourceRange.baseMipLevel = i - 1;
      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

      vkCmdPipelineBarrier(command_buffers_[0],
          VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
          0, nullptr,
          0, nullptr,
          1, &barrier);

      VkImageBlit blit = {
        .srcSubresource{
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .mipLevel = i - 1,
          .baseArrayLayer = 0,
          .layerCount = 1
        },
        .srcOffsets = { {0, 0, 0}, {mip_width, mip_height, 1} },
        .dstSubresource{
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .mipLevel = i,
          .baseArrayLayer = 0,
          .layerCount = 1,
        },
        .dstOffsets = {
          {0, 0, 0},
          { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 },
        }
      };
      vkCmdBlitImage(command_buffers_[0],
          image_, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
          image_, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          1, &blit,
          VK_FILTER_LINEAR);

      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      vkCmdPipelineBarrier(command_buffers_[0],
          VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
          0, nullptr,
          0, nullptr,
          1, &barrier);

      if (mip_width > 1) mip_width /= 2;
      if (mip_height > 1) mip_height /= 2;
  }

  barrier.subresourceRange.baseMipLevel = image_.mip_levels() - 1;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  vkCmdPipelineBarrier(command_buffers_[0],
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
      0, nullptr,
      0, nullptr,
      1, &barrier);
}

void ImageCommander::TransitImageLayout(VkImageLayout old_layout, VkImageLayout new_layout) const {
  VkImageMemoryBarrier barrier = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .oldLayout = old_layout,
    .newLayout = new_layout,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = image_,
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = image_.mip_levels(),
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };
  VkPipelineStageFlags source_stage;
  VkPipelineStageFlags destination_stage;

  if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    throw Error("unsupported layout transition");
  }
  vkCmdPipelineBarrier(
      command_buffers_[0],
      source_stage, destination_stage,
      0,
      0, nullptr,
      0, nullptr,
      1, &barrier
  );
}

void ImageCommander::CopyBuffer(const Buffer& src) const {
  const VkBufferImageCopy region = {
    .bufferOffset = 0,
    .bufferRowLength = 0,
    .bufferImageHeight = 0,
    .imageSubresource = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .mipLevel = 0,
      .baseArrayLayer = 0,
      .layerCount = 1
    },
    .imageOffset = {0, 0, 0},
    .imageExtent = {
      image_.info().extent.width,
      image_.info().extent.height,
      1
    }
  };
  vkCmdCopyBufferToImage(command_buffers_[0], src, image_, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

StagingImage::StagingImage(
  const Device& device,
  const VkExtent2D extent,
  const VkFormat format
) : Image(CreateStagingImage(device, extent, format)) {}

} // namespace vk
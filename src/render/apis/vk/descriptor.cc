#include "render/apis/vk/descriptor.h"

#include "render/apis/vk/error.h"

#include <algorithm>
#include <array>

namespace vk {

namespace {

inline VkDescriptorSetLayout CreateUniformDescriptorSetLayout(VkDevice device) {
  static constexpr VkDescriptorSetLayoutBinding layout_binding = {
    .binding = 0,
    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    .descriptorCount = 1,
    .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    .pImmutableSamplers = nullptr
  };
  constexpr VkDescriptorSetLayoutCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .bindingCount = 1,
    .pBindings = &layout_binding
  };
  VkDescriptorSetLayout layout;
  if (const VkResult result = vkCreateDescriptorSetLayout(device, &info, nullptr, &layout); result != VK_SUCCESS) {
    throw Error("failed to create descriptor set layout").WithCode(result);
  }
  return layout;
}

inline VkDescriptorSetLayout CreateSamplerDescriptorSetLayout(VkDevice device) {
  static constexpr VkDescriptorSetLayoutBinding sampler_layout_binding = {
    .binding = 0,
    .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    .descriptorCount = 1,
    .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    .pImmutableSamplers = nullptr
  };
  constexpr VkDescriptorSetLayoutCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .bindingCount = 1,
    .pBindings = &sampler_layout_binding
  };
  VkDescriptorSetLayout layout;
  if (const VkResult result = vkCreateDescriptorSetLayout(device, &info, nullptr, &layout); result != VK_SUCCESS) {
    throw Error("failed to create descriptor set layout").WithCode(result);
  }
  return layout;
}

inline VkDescriptorPool CreateDescriptorPool(VkDevice device, const size_t uniform_count, const size_t sampler_count) {
  const std::array pool_sizes = {
    VkDescriptorPoolSize{
      .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .descriptorCount = static_cast<uint32_t>(uniform_count)
    },
    VkDescriptorPoolSize{
      .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      .descriptorCount = static_cast<uint32_t>(sampler_count)
    }
  };
  const VkDescriptorPoolCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .maxSets = static_cast<uint32_t>(uniform_count + sampler_count),
    .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
    .pPoolSizes = pool_sizes.data()
  };
  VkDescriptorPool pool;
  if (const VkResult result = vkCreateDescriptorPool(device, &info, nullptr, &pool); result != VK_SUCCESS) {
    throw Error("failed to create descriptor pool").WithCode(result);
  }
  return pool;
}

} // namespace vk

DescriptorPool::DescriptorPool(VkDevice device, const size_t uniform_count, const size_t sampler_count)
  : NonDispatchableHandle(CreateDescriptorPool(device, uniform_count, sampler_count), device) {}

std::vector<VkDescriptorSet> DescriptorPool::AllocateDescriptorSets(VkDescriptorSetLayout set_layout, const size_t count) const {
  const std::vector layouts(count, set_layout);

  const VkDescriptorSetAllocateInfo info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .descriptorPool = handle_,
    .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
    .pSetLayouts = layouts.data()
  };
  std::vector<VkDescriptorSet> sets(count);
  if (const VkResult result = vkAllocateDescriptorSets(parent_, &info, sets.data()); result != VK_SUCCESS) {
    throw Error("failed to allocate command buffers").WithCode(result);
  }
  return sets;
}

UniformDescriptorSetLayout::UniformDescriptorSetLayout(VkDevice device)
  : NonDispatchableHandle(CreateUniformDescriptorSetLayout(device), device) {}

SamplerDescriptorSetLayout::SamplerDescriptorSetLayout(VkDevice device)
  : NonDispatchableHandle(CreateSamplerDescriptorSetLayout(device), device) {}

UniformDescriptorSet::UniformDescriptorSet(const Device& device, VkDescriptorSet descriptor_set, const VkDeviceSize size) noexcept
: IdentityHandle(descriptor_set),
  buffer_(
    device,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    size
  ),
  device_(device),
  size_(size) {}

void UniformDescriptorSet::Update() const noexcept {
  const VkDescriptorBufferInfo buffer_info = {
    .buffer = buffer_,
    .offset = 0,
    .range = size_
  };
  const VkWriteDescriptorSet descriptor_write = {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .dstSet = static_cast<VkDescriptorSet>(*this),
    .dstBinding = 0,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    .pBufferInfo = &buffer_info
  };
  vkUpdateDescriptorSets(device_, 1, &descriptor_write, 0, nullptr);
}

SamplerDescriptorSet::SamplerDescriptorSet(const Device& device, Image&& image, VkDescriptorSet descriptor_set) noexcept
  : IdentityHandle(descriptor_set),
    sampler_(device, VK_SAMPLER_MIPMAP_MODE_LINEAR, image.mip_levels()),
    image_(std::move(image)),
    device_(device) {}

void SamplerDescriptorSet::Update() const noexcept {
  const VkDescriptorImageInfo image_info = {
      .sampler = sampler_,
      .imageView = image_.view(),
      .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
  };
  const VkWriteDescriptorSet descriptor_write = {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .dstSet = static_cast<VkDescriptorSet>(*this),
    .dstBinding = 0,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    .pImageInfo = &image_info
  };
  vkUpdateDescriptorSets(device_, 1, &descriptor_write, 0, nullptr);
}

} // namespace vk
#ifndef RENDER_APIS_VK_DESCRIPTOR_H_
#define RENDER_APIS_VK_DESCRIPTOR_H_

#include "render/apis/vk/buffer.h"
#include "render/apis/vk/image.h"
#include "render/apis/vk/handle.h"
#include "render/apis/vk/sampler.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

struct DescriptorPool : NonDispatchableHandle<VkDescriptorPool, VkDevice, vkDestroyDescriptorPool> {
  DECLARE_DEFAULT_NO_COPY_CLASS(DescriptorPool);
  DescriptorPool(VkDevice device, size_t uniform_count, size_t sampler_count);

  [[nodiscard]]std::vector<VkDescriptorSet> AllocateDescriptorSets(VkDescriptorSetLayout set_layout, size_t count) const;
};

struct UniformDescriptorSetLayout : NonDispatchableHandle<VkDescriptorSetLayout, VkDevice, vkDestroyDescriptorSetLayout> {
  DECLARE_DEFAULT_NO_COPY_CLASS(UniformDescriptorSetLayout);
  explicit UniformDescriptorSetLayout(VkDevice device);
};

class UniformDescriptorSet final : public IdentityHandle<VkDescriptorSet> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(UniformDescriptorSet);

  explicit UniformDescriptorSet(const Device& device, VkDescriptorSet descriptor_set, VkDeviceSize size) noexcept;

  void Update() const noexcept;

  [[nodiscard]]const Buffer& buffer() const noexcept {
    return buffer_;
  }
private:
  Buffer buffer_;
  VkDevice device_ = VK_NULL_HANDLE;
  VkDeviceSize size_ = 0;
};

struct SamplerDescriptorSetLayout : NonDispatchableHandle<VkDescriptorSetLayout, VkDevice, vkDestroyDescriptorSetLayout>  {
  DECLARE_DEFAULT_NO_COPY_CLASS(SamplerDescriptorSetLayout);
  explicit SamplerDescriptorSetLayout(VkDevice device);
};

class SamplerDescriptorSet final : public IdentityHandle<VkDescriptorSet> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(SamplerDescriptorSet);

  explicit SamplerDescriptorSet(const Device& device, Image&& image, VkDescriptorSet descriptor_set) noexcept;

  void Update() const noexcept;

  [[nodiscard]] const Sampler& sampler() const noexcept {
    return sampler_;
  }

  [[nodiscard]] const Image& image() const noexcept {
    return image_;
  }
private:
  Sampler sampler_;
  Image image_;
  VkDevice device_ = VK_NULL_HANDLE;
};

} // namespace vk

#endif // RENDER_APIS_VK_DESCRIPTOR_H_

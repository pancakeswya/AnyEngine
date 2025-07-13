#ifndef RENDER_APIS_VK_OBJECT_H_
#define RENDER_APIS_VK_OBJECT_H_

#include "render/object.h"
#include "render/apis/vk/buffer.h"
#include "render/apis/vk/image.h"
#include "render/apis/vk/descriptor.h"
#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

struct Vertex : render::Vertex {
  static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
  static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
};

using Index = render::Index;

template<typename Tp>
struct IndexType;

template<>
struct IndexType<uint16_t> {
  static constexpr VkIndexType value = VK_INDEX_TYPE_UINT16;
};

template<>
struct IndexType<uint32_t> {
  static constexpr VkIndexType value = VK_INDEX_TYPE_UINT32;
};

using Uniforms = render::Uniforms;

class Object final : public render::Object {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Object);

  Object(
    const Device& device,
    const UniformDescriptorSetLayout& uniform_descriptor_set_layout,
    const SamplerDescriptorSetLayout& sampler_descriptor_set_layout,
    const render::GeometryInfo& geometry_info,
    const std::vector<render::TextureIndices>& texture_indices,
    StagingBuffer&& vertices_buffer,
    StagingBuffer&& indices_buffer,
    std::vector<StagingImage>&& images,
    size_t frame_count
  );


  void SetFrameIndex(size_t frame_index) noexcept;

  [[nodiscard]]render::Uniforms* uniforms() const noexcept override {
    return uniforms_mapped_[frame_index_];
  }

  [[nodiscard]] const StagingBuffer& vertices() const noexcept {
    return vertices_;
  }
  [[nodiscard]] const StagingBuffer& indices() const noexcept {
    return indices_;
  }

  [[nodiscard]] const std::vector<UniformDescriptorSet>& uniform_descriptor_sets() const noexcept {
    return uniform_descriptor_sets_;
  }

  [[nodiscard]] const std::vector<SamplerDescriptorSet>& sampler_descriptor_sets() const noexcept {
    return sampler_descriptor_sets_;
  }
private:
  StagingBuffer vertices_;
  StagingBuffer indices_;

  DescriptorPool descriptor_pool_;

  std::vector<UniformDescriptorSet> uniform_descriptor_sets_;
  std::vector<SamplerDescriptorSet> sampler_descriptor_sets_;
  std::vector<Uniforms*> uniforms_mapped_;

  size_t frame_index_ = 0;
};

} // namespace vk

#endif // RENDER_APIS_VK_OBJECT_H_

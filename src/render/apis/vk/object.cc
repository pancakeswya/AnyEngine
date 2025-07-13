#include "render/apis/vk/object.h"

#include <utility>

namespace vk {

namespace {

std::vector<UniformDescriptorSet> CreateUniformDescriptorSets(const Device& device, const std::vector<VkDescriptorSet>& descriptor_sets) {
  std::vector<UniformDescriptorSet> uniform_descriptor_sets;
  uniform_descriptor_sets.reserve(descriptor_sets.size());
  for(VkDescriptorSet descriptor_set : descriptor_sets) {
    UniformDescriptorSet uniform_descriptor_set(device, descriptor_set, sizeof(Uniforms));
    uniform_descriptor_set.Update();
    uniform_descriptor_sets.emplace_back(std::move(uniform_descriptor_set));
  }
  return uniform_descriptor_sets;
}

std::vector<SamplerDescriptorSet> CreateSamplerDescriptorSets(const Device& device, const std::vector<VkDescriptorSet>& descriptor_sets, std::vector<StagingImage>&& images) {
  std::vector<SamplerDescriptorSet> sampler_descriptor_sets;
  sampler_descriptor_sets.reserve(images.size());
  for(size_t i = 0; i < images.size(); ++i) {
    SamplerDescriptorSet sampler_descriptor_set(device, std::move(images[i]), descriptor_sets[i]);
    sampler_descriptor_set.Update();
    sampler_descriptor_sets.emplace_back(std::move(sampler_descriptor_set));
  }
  return sampler_descriptor_sets;
}

} // namespace

std::vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions() {
  return {
      {
        .binding = 0,
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
      }
  };
}

std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions() {
  return {
    {
      .location = 0,
      .binding = 0,
      .format = VK_FORMAT_R32G32B32_SFLOAT,
      .offset = offsetof(Vertex, pos)
    },
    {
      .location = 1,
      .binding = 0,
      .format = VK_FORMAT_R32G32B32_SFLOAT,
      .offset = offsetof(Vertex, normal)
    },
    {
      .location = 2,
      .binding = 0,
      .format = VK_FORMAT_R32G32_SFLOAT,
      .offset = offsetof(Vertex, tex_coord)
    }
  };
}

Object::Object(
  const Device& device,
  const UniformDescriptorSetLayout& uniform_descriptor_set_layout,
  const SamplerDescriptorSetLayout& sampler_descriptor_set_layout,
  const render::GeometryInfo& geometry_info,
  const std::vector<render::TextureIndices>& texture_indices,
  StagingBuffer&& vertices_buffer,
  StagingBuffer&& indices_buffer,
  std::vector<StagingImage>&& images,
  const size_t frame_count
) : render::Object(geometry_info, texture_indices),
    vertices_(std::move(vertices_buffer)),
    indices_(std::move(indices_buffer)),
    descriptor_pool_(device, frame_count, images.size()),
    uniform_descriptor_sets_(
      CreateUniformDescriptorSets(
        device,
        descriptor_pool_.AllocateDescriptorSets(uniform_descriptor_set_layout, frame_count)
      )
    ),
    sampler_descriptor_sets_(
      CreateSamplerDescriptorSets(
        device,
        descriptor_pool_. AllocateDescriptorSets(sampler_descriptor_set_layout, images.size()),
        std::move(images)
      )
    ) {
  uniforms_mapped_.reserve(uniform_descriptor_sets_.size());
  for(const UniformDescriptorSet& uniform_descriptor_set : uniform_descriptor_sets_) {
    uniforms_mapped_.emplace_back(uniform_descriptor_set.buffer().memory().Map<Uniforms>());
  }
}

void Object::SetFrameIndex(const size_t frame_index) noexcept {
  frame_index_ = frame_index;
}

} // namespace vk
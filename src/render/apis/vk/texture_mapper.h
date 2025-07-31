#ifndef RENDER_APIS_VK_TEXTURE_MAPPER_H_
#define RENDER_APIS_VK_TEXTURE_MAPPER_H_

#include "render/texture_mapper.h"

#include <vulkan/vulkan.h>

namespace vk {

struct TextureMapper : virtual render::TextureMapper {
  [[nodiscard]] virtual VkFormat format() const noexcept = 0;
};

} // namespace vk

#endif // RENDER_APIS_VK_TEXTURE_MAPPER_H_

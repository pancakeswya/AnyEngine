#ifndef SDL_RENDERER_VK_TEXTURE_MAPPER_H_
#define SDL_RENDERER_VK_TEXTURE_MAPPER_H_

#include "sdl/image/texture_mapper.h"
#include "sdl/image/texture_mapper_mock.h"
#include "render/apis/vk/texture_mapper.h"

namespace sdl::vk {

struct TextureMapperMock final : sdl::TextureMapperMock, ::vk::TextureMapper {
  [[nodiscard]] VkFormat format() const noexcept override;
};

class TextureMapper final : public sdl::TextureMapper, public ::vk::TextureMapper {
public:
  explicit TextureMapper(const std::string& filepath);
  ~TextureMapper() override = default;

  [[nodiscard]] VkFormat format() const noexcept override {
    return format_;
  }
private:
  VkFormat format_ = VK_FORMAT_UNDEFINED;
};

} // namespace sdk::vk

#endif // SDL_RENDERER_VK_TEXTURE_MAPPER_H_

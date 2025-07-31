#ifndef SDL_RENDERER_TEXTURE_MAPPER_H_
#define SDL_RENDERER_TEXTURE_MAPPER_H_

#include "sdl/image/texture_mapper.h"
#include "sdl/image/texture_mapper_mock.h"
#include "render/apis/gl/texture_mapper.h"

namespace sdl::gl {

struct TextureMapperMock final : sdl::TextureMapperMock, ::gl::TextureMapper {
  [[nodiscard]] GLenum format() const noexcept override;
};

class TextureMapper final : public sdl::TextureMapper, public ::gl::TextureMapper {
public:
  explicit TextureMapper(const std::string& filepath);
  ~TextureMapper() override = default;

  [[nodiscard]] GLenum format() const noexcept override {
    return format_;
  }
private:
  GLenum format_ = GL_INVALID_ENUM;
};

} // namespace sdk::gl

#endif // SDL_RENDERER_TEXTURE_MAPPER_H_

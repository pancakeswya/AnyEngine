#ifndef RENDER_APIS_GL_OBJECT_H_
#define RENDER_APIS_GL_OBJECT_H_

#include "render/object.h"
#include "render/apis/gl/buffer.h"
#include "render/apis/gl/texture.h"

#include <vector>

namespace gl {

class Object final : public render::Object {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Object);

  Object(
    const render::GeometryInfo& geometry_info,
    const std::vector<render::TextureIndices>& texture_indices,
    GLuint program,
    Buffer&& vertices,
    Buffer&& indices,
    std::vector<Texture>&& textures
  );

  void UpdateUniforms(const render::Uniforms* uniforms) override;

  [[nodiscard]] const std::vector<Texture>& textures() const noexcept {
    return textures_;
  }
private:
  GLint model_location_ = GL_INVALID_INDEX;
  GLint view_location_ = GL_INVALID_INDEX;
  GLint projection_location_ = GL_INVALID_INDEX;

  Buffer vertices_;
  Buffer indices_;

  std::vector<Texture> textures_;
};

} // namespace gl

#endif // RENDER_APIS_GL_OBJECT_H_

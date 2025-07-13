#ifndef RENDER_APIS_GL_OBJECT_H_
#define RENDER_APIS_GL_OBJECT_H_

#include "render/object.h"
#include "render/apis/gl/buffer.h"
#include "render/apis/gl/texture.h"

#include <vector>

#ifndef USE_BUFFER_MAP
#include <memory>
#endif

namespace gl {

class Object final : public render::Object {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Object);

  Object(
    const render::GeometryInfo& geometry_info,
    const std::vector<render::TextureIndices>& texture_indices,
#ifdef USE_VAO
    VertexArrayBuffer&& vao,
#endif
    Buffer&& vertices,
    Buffer&& indices,
    Buffer&& uniforms,
    std::vector<Texture>&& textures
  );

  [[nodiscard]] render::Uniforms* uniforms() const noexcept override {
#ifdef USE_BUFFER_MAP
    return uniforms_mapped_;
#else
    render::Uniforms* uniforms = uniforms_mapped_.get();
    uniforms_.Copy(uniforms, sizeof(render::Uniforms));
    return uniforms;
#endif
  }

  [[nodiscard]] const std::vector<Texture>& textures() const noexcept {
    return textures_;
  }
private:
#ifdef USE_VAO
  VertexArrayBuffer vao_;
#endif
  Buffer vertices_;
  Buffer indices_;
  Buffer uniforms_;
#ifdef USE_BUFFER_MAP
  render::Uniforms*
#else
  std::unique_ptr<render::Uniforms>
#endif
    uniforms_mapped_;

  std::vector<Texture> textures_;
};

} // namespace gl

#endif // RENDER_APIS_GL_OBJECT_H_

#include "render/apis/gl/object.h"

#include <utility>
#include <cstring>

namespace gl {

Object::Object(
  const render::GeometryInfo& geometry_info,
  const std::vector<render::TextureIndices>& texture_indices,
#ifdef USE_VAO
  VertexArrayBuffer&& vao,
#endif
  Buffer&& vertices,
  Buffer&& indices,
  Buffer&& uniforms,
  std::vector<Texture>&& textures
) : render::Object(geometry_info, texture_indices),
#ifdef USE_VAO
    vao_(std::move(vao)),
#endif
    vertices_(std::move(vertices)),
    indices_(std::move(indices)),
    uniforms_(std::move(uniforms)),
    uniforms_mapped_(
#ifdef USE_BUFFER_MAP
      uniforms_.Map<render::Uniforms>(Buffer::kWriteBit)
#else
      std::make_unique<render::Uniforms>()
#endif
    ),
    textures_(std::move(textures)) {}

} // namespace gl
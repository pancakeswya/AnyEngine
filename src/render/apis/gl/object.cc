#include "render/apis/gl/object.h"

#include <glm/gtc/type_ptr.hpp>

namespace gl {

Object::Object(
  const render::GeometryInfo& geometry_info,
  const std::vector<render::TextureIndices>& texture_indices,
  const GLuint program,
  Buffer&& vertices,
  Buffer&& indices,
  std::vector<Texture>&& textures
) : render::Object(geometry_info, texture_indices),
    model_location_(glGetUniformLocation(program, "ubo.model")),
    view_location_(glGetUniformLocation(program, "ubo.view")),
    projection_location_(glGetUniformLocation(program, "ubo.proj")),
    vertices_(std::move(vertices)),
    indices_(std::move(indices)),
    textures_(std::move(textures)) {}

void Object::UpdateUniforms(const render::Uniforms* uniforms) {
  const auto&[model, view, proj] = *uniforms;

  glUniformMatrix4fv(model_location_, 1, GL_FALSE, glm::value_ptr(model[0]));
  glUniformMatrix4fv(view_location_, 1, GL_FALSE, glm::value_ptr(view[0]));
  glUniformMatrix4fv(projection_location_, 1, GL_FALSE, glm::value_ptr(proj[0]));
}

} // namespace gl
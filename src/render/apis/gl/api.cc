#include "render/apis/gl/api.h"

#include "render/apis/gl/check.h"
#include "render/apis/gl/shaders.h"

namespace gl {

namespace {

GLenum MapFormat(const SDL_PixelFormat format) noexcept {
  switch (format) {
    case SDL_PIXELFORMAT_RGBA32:
      return GL_RGBA;
    default:
      break;
  }
  return GL_INVALID_ENUM;
}

} // namespace

Api::Api(SDL_Window* window)
  : context_(window),
    program_(GetShaderInfos()),
    window_(window) {}

void Api::OnResize(int width, int height) {
  SDL_GetWindowSizeInPixels(window_, &width, &height);
  GL_CHECK(glViewport(0, 0, width, height));
}

void Api::RenderFrame() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  size_t prev_offset = 0;

  for(const Object& object : objects_) {
    for(const auto[index, offset] : object.texture_indices()) {
      object.textures().at(index).Bind();
      GL_CHECK(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(offset - prev_offset), GL_UNSIGNED_INT, reinterpret_cast<void*>(prev_offset * sizeof(GLuint))));
      prev_offset = offset;
    }
  }
  glFinish();
  SDL_GL_SwapWindow(window_);
}

render::Object* Api::LoadObject(
    render::GeometryTransferer& geometry_transferer,
    std::vector<std::unique_ptr<render::TextureMapper>>& texture_mappers) {
  const render::GeometryInfo geometry_info = geometry_transferer.info();
  const std::vector<render::TextureIndices>& texture_indices = geometry_transferer.texture_indices();

#ifdef USE_VAO
  VertexArrayBuffer array_buffer;
  array_buffer.Bind();
#endif
  Buffer indices_buffer(GL_ELEMENT_ARRAY_BUFFER);
  indices_buffer.Bind();
  indices_buffer.Allocate(sizeof(render::Index) * geometry_info.index_count, GL_STATIC_DRAW);

  Buffer vertices_buffer(GL_ARRAY_BUFFER);
  vertices_buffer.Bind();
  vertices_buffer.Allocate(sizeof(render::Vertex) * geometry_info.vertex_count, GL_STATIC_DRAW);

  Buffer uniform_buffer(GL_UNIFORM_BUFFER);
  uniform_buffer.Bind();
  uniform_buffer.Allocate(sizeof(render::Uniforms), GL_DYNAMIC_DRAW);

#ifdef USE_BUFFER_MAP
  const auto indices = indices_buffer.Map<render::Index>(Buffer::kWriteBit);
  const auto vertices = vertices_buffer.Map<render::Vertex>(Buffer::kWriteBit);
  geometry_transferer.Transfer(vertices, indices);
#else
  std::vector<render::Vertex> vertices(vertices_buffer.allocated_size());
  std::vector<render::Index> indices(indices_buffer.allocated_size());
  geometry_transferer.Transfer(vertices.data(), indices.data());
#endif

  const GLuint pos_loc = glGetAttribLocation(program_, "inPosition");
  GL_CHECK(glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), nullptr));
  GL_CHECK(glEnableVertexAttribArray(pos_loc));

  const GLuint normal_loc = glGetAttribLocation(program_, "inNormal");
  GL_CHECK(glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(GLfloat))));
  GL_CHECK(glEnableVertexAttribArray(normal_loc));

  const GLuint tex_loc = glGetAttribLocation(program_, "inTexCoord");
  GL_CHECK(glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE,  8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(GLfloat))));
  GL_CHECK(glEnableVertexAttribArray(tex_loc));

  const GLuint ubo_index = glGetUniformBlockIndex(program_, "ubo");
  GL_CHECK(glUniformBlockBinding(program_, ubo_index, 0));
  GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, ubo_index, uniform_buffer));

#ifdef USE_BUFFER_MAP
  indices_buffer.Unmap();
  vertices_buffer.Unmap();
#else
  vertices_buffer.Copy(vertices.data(), vertices.size());
  indices_buffer.Copy(indices.data(), indices.size());
#endif
  std::vector<Texture> textures;
  textures.reserve(texture_mappers.size());
  for (const std::unique_ptr<render::TextureMapper>& texture_mapper : texture_mappers) {
    const auto[width, height, format] = texture_mapper->info();

    const SDL_PixelFormat sdl_format = MapFormat(format) == GL_INVALID_ENUM ? SDL_PIXELFORMAT_RGBA32 : format;
    const GLenum gl_format = MapFormat(sdl_format);

    const uint8_t* pixels = texture_mapper->Map(sdl_format);
    textures.emplace_back(gl_format, pixels, width, height);
  }
  objects_.emplace_back(
    geometry_info,
    texture_indices,
#ifdef USE_VAO
    std::move(array_buffer),
#endif
    std::move(vertices_buffer),
    std::move(indices_buffer),
    std::move(uniform_buffer),
    std::move(textures)
  );
  return objects_.data() + objects_.size() - 1;
}

} // namespace gl
#include "render/apis/gl/api.h"

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

void Api::OnResize(const int width, const int height) {
  glViewport(0, 0, width, height);
}

void Api::RenderFrame() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  size_t prev_offset = 0;

  for(const Object& object : objects_) {
    for(const auto[index, offset] : object.texture_indices()) {
      glBindTexture(GL_TEXTURE_2D, object.textures().at(index));
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(offset - prev_offset), GL_UNSIGNED_INT, reinterpret_cast<void*>(prev_offset * sizeof(GLuint)));
      prev_offset = offset;
    }
  }
  glFinish();
  SDL_GL_SwapWindow(window_);
}

render::Object* Api::LoadObject(
    render::GeometryTransferer& geometry_transferer,
    std::vector<std::unique_ptr<render::TextureTransferer>>& texture_transferers) {
  const render::GeometryInfo geometry_info = geometry_transferer.info();
  const std::vector<render::TextureIndices>& texture_indices = geometry_transferer.texture_indices();

  Buffer indices_buffer(GL_ELEMENT_ARRAY_BUFFER);
  indices_buffer.Bind();
  indices_buffer.Allocate(sizeof(render::Index) * geometry_info.index_count, GL_STATIC_DRAW);

  Buffer vertices_buffer(GL_ARRAY_BUFFER);
  vertices_buffer.Bind();
  vertices_buffer.Allocate(sizeof(render::Vertex) * geometry_info.vertex_count, GL_STATIC_DRAW);

  const auto indices = indices_buffer.Map<render::Index>(GL_WRITE_ONLY);
  const auto vertices = vertices_buffer.Map<render::Vertex>(GL_WRITE_ONLY);

  geometry_transferer.Transfer(vertices, indices);

  const GLuint pos_loc = glGetAttribLocation(program_, "inPosition");
  glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(pos_loc);

  const GLuint normal_loc = glGetAttribLocation(program_, "inNormal");
  glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(normal_loc);

  const GLuint tex_loc = glGetAttribLocation(program_, "inTexCoord");
  glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE,  8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(tex_loc);

  indices_buffer.Unmap();
  vertices_buffer.Unmap();

  std::vector<Texture> textures;
  textures.reserve(texture_transferers.size());
  for (const std::unique_ptr<render::TextureTransferer>& texture_transferer : texture_transferers) {
    const auto[width, height, format] = texture_transferer->info();

    const SDL_PixelFormat sdl_format = MapFormat(format) == GL_INVALID_ENUM ? SDL_PIXELFORMAT_RGBA32 : format;
    const GLenum gl_format = MapFormat(sdl_format);

    std::vector<uint8_t> pixels(width * height * SDL_BYTESPERPIXEL(sdl_format));
    texture_transferer->Transfer(pixels.data(), sdl_format);
    textures.emplace_back(gl_format, pixels.data(), width, height);
  }
  objects_.emplace_back(
    geometry_info,
    texture_indices,
    program_,
    std::move(vertices_buffer),
    std::move(indices_buffer),
    std::move(textures)
  );
  return objects_.data() + objects_.size() - 1;
}

} // namespace gl
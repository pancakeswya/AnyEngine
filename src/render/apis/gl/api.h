#ifndef RENDER_APIS_GL_API_H_
#define RENDER_APIS_GL_API_H_

#include "render/api.h"
#include "render/apis/gl/loader.h"
#include "render/apis/gl/object.h"
#include "render/apis/gl/shader.h"
#include "render/apis/gl/window.h"
#include "render/apis/gl/texture_mapper.h"

#include <vector>
#include <memory>

namespace gl {

class Api final : public render::Api<TextureMapper> {
public:
#ifdef USE_OPENGL_ES3
  static constexpr auto kGlSlVersion = "#version 300 es\n";
#else
  static constexpr auto kGlSlVersion = "#version 150\n";
#endif

  explicit Api(const Window& window);
  ~Api() override = default;

  void OnResize(int width, int height) override;

  void RenderFrame() override;
  render::Object* LoadObject(
      render::GeometryTransferer& geometry_transferer,
      std::vector<std::unique_ptr<TextureMapper>>& texture_mappers) override;
private:
  ShaderProgram program_;
  std::vector<Object> objects_;

  const Window& window_;
};

} // namespace gl

#endif // RENDER_APIS_GL_API_H_

#ifndef RENDER_APIS_GL_API_H_
#define RENDER_APIS_GL_API_H_

#include "render/api.h"
#include "render/apis/gl/loader.h"
#include "render/apis/gl/context.h"
#include "render/apis/gl/object.h"
#include "render/apis/gl/shader.h"
#include "render/apis/gl/gui.h"

#include <SDL3/SDL.h>

#include <vector>
#include <memory>

namespace gl {

class Api final : public render::Api {
public:
  explicit Api(SDL_Window* window, float scale_factor);
  ~Api() override = default;

  void OnResize(int width, int height) override;

  void RenderFrame() override;
  render::Object* LoadObject(
      render::GeometryTransferer& geometry_transferer,
      std::vector<std::unique_ptr<render::TextureMapper>>& texture_mappers) override;

  render::GuiRenderer* GetGuiRenderer() noexcept override {
    return &gui_renderer_;
  }
private:
  Context context_;
  ShaderProgram program_;
  std::vector<Object> objects_;

  SDL_Window* window_;
  GuiRenderer gui_renderer_;
};

} // namespace gl

#endif // RENDER_APIS_GL_API_H_

#ifndef RENDER_APIS_GL_API_H_
#define RENDER_APIS_GL_API_H_

#include "render/api.h"
#include "render/apis/gl/loader.h"
#include "render/apis/gl/context.h"
#include "render/apis/gl/object.h"
#include "render/apis/gl/shader.h"

#include <SDL3/SDL.h>

#include <vector>
#include <memory>

namespace gl {

class Api final : public render::Api {
public:
  explicit Api(SDL_Window* window);
  ~Api() override = default;

  void OnResize(int width, int height) override;

  void RenderFrame() override;
  render::Object* LoadObject(
      render::GeometryTransferer& geometry_transferer,
      std::vector<std::unique_ptr<render::TextureTransferer>>&
          texture_transferers) override;
private:
  Context context_;
  ShaderProgram program_;
  std::vector<Object> objects_;

  SDL_Window* window_;
};

} // namespace gl

#endif // RENDER_APIS_GL_API_H_

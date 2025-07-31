#ifndef SDL_RENDERER_H_
#define SDL_RENDERER_H_

#include "render/object.h"
#include "render/geometry_transferer.h"

#include <vector>
#include <string>

namespace sdl {

struct Renderer {
  virtual ~Renderer() = default;

  virtual void RenderFrame() = 0;
  virtual render::Object* LoadObject(
    render::GeometryTransferer& geometry_transferer,
    const std::vector<std::string>& texture_paths
  ) = 0;
  virtual void OnResize(int width, int height) = 0;
};

} // namespace sdl

#endif // SDL_RENDERER_H_

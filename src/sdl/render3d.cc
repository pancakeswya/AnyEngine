#include "sdl/render3d.h"

#include "fs/path.h"
#include "sdl/error.h"
#include "sdl/gui/gui.h"
#include "render/parsers/obj/object_parser.h"

namespace sdl {

Render3D::Render3D(SDL_Window* window, const float scale_factor)
  : renderer_(window, scale_factor) {}

void Render3D::ProcessEvent(const SDL_Event* event) {
  GuiRenderer::ProcessEvent(event);
  if (event->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
    renderer_.OnResize(event->window.data1, event->window.data2);
  }
}

void Render3D::RenderFrame() {
  renderer_.RenderFrame();
}

render::Object* Render3D::LoadObject(const std::string& filepath) {
  const std::string path = (fs::BasePath() / filepath).string();
  std::vector<std::string> texture_paths;
  render::GeometryTransferer& geometry_transferer = parser_.Parse(path, texture_paths);
  return renderer_.LoadObject(geometry_transferer, texture_paths);
}

} // namespace sdl
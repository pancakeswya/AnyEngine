#include "app.h"

#include "render/transferers/sdl/texture_transferer.h"
#include "io/file.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

namespace {

SDL_Window* CreateWindow(const SDL_WindowFlags window_flags) {
  const float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_Window* window = SDL_CreateWindow(
    "3D Render",
    static_cast<int>(1280 * main_scale),
    static_cast<int>(720 * main_scale),
    SDL_WINDOW_RESIZABLE |
    SDL_WINDOW_HIGH_PIXEL_DENSITY |
    window_flags
  );
  if (window == nullptr) {
    throw App::Error(std::string("Failed to create window") + SDL_GetError());
  }
  return window;
}

render::Object* LoadObject(const std::string& filename,
                           const render::ApiHandle& api_handle,
                           const render::ObjectParserHandle& object_parser_handle) {
  std::vector<std::string> texture_paths;
  render::GeometryTransferer& geometry_transferer = object_parser_handle->Parse((io::BasePath() / filename).string(), texture_paths);
  std::vector<std::unique_ptr<render::TextureTransferer>> texture_transferers;
  texture_transferers.reserve(texture_paths.size());
  for (const std::string& texture_path : texture_paths) {
    texture_transferers.emplace_back(std::make_unique<sdl::TextureTransferer>(texture_path));
  }
  return api_handle->LoadObject(geometry_transferer, texture_transferers);
}

} // namespace

App::App()
    : api_plugin_("libvk_api"),
      window_(CreateWindow(api_plugin_.GetWindowFlags())),
      object_parser_plugin_("libobj_parser") {}

void App::Init() {
  api_handle_ = api_plugin_.CreateHandle(window_);
  object_parser_handle_ = object_parser_plugin_.CreateHandle();
  object_ = LoadObject("obj/tommy/tommy.obj", api_handle_, object_parser_handle_);
}

SDL_AppResult App::HandleEvent(const SDL_Event* event) const {
  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
    case SDL_EVENT_WINDOW_RESIZED:
      api_handle_->OnResize(event->window.data1, event->window.data2);
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult App::Iterate() const {
  api_handle_->RenderFrame();

  static auto start_time = std::chrono::high_resolution_clock::now();

  const auto current_time = std::chrono::high_resolution_clock::now();
  const float time = std::chrono::duration<float>(current_time - start_time).count();

  render::Uniforms uniforms = {};
  uniforms.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  uniforms.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  int width, height;
  SDL_GetWindowSize(window_, &width, &height);
  uniforms.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 10.0f);
  uniforms.proj[1][1] *= -1;

  object_->UpdateUniforms(&uniforms);

  return SDL_APP_CONTINUE;
}


#include "app.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <SDL3/SDL_log.h>

#include "fs/path.h"
#include "render/mappers/sdl/texture_mapper.h"
#include "render/mappers/mock/texture_mapper.h"
#include "resource/scope_exit.h"

#include <filesystem>

namespace {

render::Object* LoadObject(const std::string& filename,
                           render::Api* api_handle,
                           render::ObjectParser* object_parser_handle) {
  const std::string path = (fs::BasePath() / filename).string();
  std::vector<std::string> texture_paths;
  render::GeometryTransferer& geometry_transferer = object_parser_handle->Parse(path, texture_paths);
  std::vector<std::unique_ptr<render::TextureMapper>> texture_mappers;
  texture_mappers.reserve(texture_paths.size());
  if (texture_paths.empty()) {
    texture_mappers.emplace_back(std::make_unique<mock::TextureMapper>());
  } else {
    for (const std::string& texture_path : texture_paths) {
      try {
        std::filesystem::path texture_filepath(texture_path);
        texture_mappers.emplace_back(std::make_unique<sdl::TextureMapper>(texture_filepath.lexically_normal().string()));
      } catch (const std::exception& exception) {
        SDL_Log("Failed to load texture mapper: %s\n", exception.what());
        texture_mappers.emplace_back(std::make_unique<mock::TextureMapper>());
      }
    }
  }
  return api_handle->LoadObject(geometry_transferer, texture_mappers);
}

} // namespace

App::App()
    : api_plugin_("libvk_api"),
      object_parser_plugin_("libobj_parser") {}

App::~App() {
  object_parser_plugin_.DestroyObjectParser(object_parser_);
  api_plugin_.Destroy(api_, window_);
}

void App::Init() {
  api_ = api_plugin_.CreateApiAndWindow(
    kTitle.data(),
    1280,
    720,
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY,
    window_
  );
  resource::scope_exit render_guard([this] {
    api_plugin_.Destroy(api_, window_);
  });

  object_parser_ = object_parser_plugin_.CreateObjectParser();
  resource::scope_exit object_parser_guard([this] {
    object_parser_plugin_.DestroyObjectParser(object_parser_);
  });

  object_ = LoadObject(
    "obj/Madara Uchiha/obj/Madara_Uchiha.obj",
    api_,
    object_parser_
  );
  resource::release_all(render_guard, object_parser_guard);
}

SDL_AppResult App::HandleEvent(const SDL_Event* event) const {
  api_->GetGuiRenderer()->ProcessEvent(event);
  switch (event->type) {
    case SDL_EVENT_TERMINATING:
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      api_->OnResize(event->window.data1, event->window.data2);
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult App::Iterate() {
  api_->RenderFrame();

  static auto start_time = std::chrono::high_resolution_clock::now();

  const auto current_time = std::chrono::high_resolution_clock::now();
  const float time = std::chrono::duration<float>(current_time - start_time).count();

  render::Uniforms* uniforms = object_->uniforms();
  uniforms->model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  uniforms->view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  int width, height;
  SDL_GetWindowSize(window_, &width, &height);
  uniforms->proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 10.0f);
  uniforms->proj[1][1] *= -1;

  object_->UpdateUniforms();

  return SDL_APP_CONTINUE;
}



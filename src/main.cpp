#include "render/api.h"
#include "render/api_plugin_import.h"
#include "render/object.h"
#include "render/object_parser.h"
#include "render/object_parser_plugin.h"
#include "render/object_parser_plugin_import.h"
#include "render/transferers/sdl/texture_transferer.h"
#include "resource/scope_exit.h"
#include "io/file.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define LOG_ERROR(fmt, ...) SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, fmt"\n", ## __VA_ARGS__);
#define LOG_INFO(fmt, ...) SDL_LogInfo(SDL_LOG_CATEGORY_CUSTOM, fmt"\n", ## __VA_ARGS__);

struct App {
  SDL_Window* window;
  render::Object& render_object;
  render::ApiPlugin api_plugin;
  render::ApiHandle render_api;
  render::ObjectParserPlugin object_parser_plugin;
  render::ObjectParserHandle object_parser;

  SDL_AppResult app_quit;

  App(SDL_Window* window, render::ApiPlugin&& api_plugin, render::ApiHandle&& render_api, render::ObjectParserPlugin&& object_parser_plugin, render::ObjectParserHandle&& object_parser, render::Object& render_object)
    : window(window),
      render_object(render_object),
      api_plugin(std::move(api_plugin)),
      render_api(std::move(render_api)),
      object_parser_plugin(std::move(object_parser_plugin)),
      object_parser(std::move(object_parser)),
      app_quit(SDL_APP_CONTINUE) {}

  ~App() = default;
};

SDL_AppResult SDL_AppInit(
  void** appstate,
  [[maybe_unused]]int argc,
  [[maybe_unused]]char* argv[]
) {
  if (!SDL_Init(SDL_INIT_VIDEO)){
    LOG_ERROR("SDL_Init failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  resource::scope_exit sdl_guard([] {
    SDL_Quit();
  });
  render::ApiPlugin plugin("libvk_api");
  const float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_Window* window = SDL_CreateWindow(
    "3D Render",
    static_cast<int>(1280 * main_scale),
    static_cast<int>(720 * main_scale),
    SDL_WINDOW_RESIZABLE |
    SDL_WINDOW_HIGH_PIXEL_DENSITY |
    plugin.GetWindowFlags()
  );
  if (window == nullptr) {
    LOG_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  auto render_api = plugin.CreateHandle(window);

  render::ObjectParserPlugin object_parser_plugin("libobj_parser");
  render::ObjectParserHandle object_parser = object_parser_plugin.CreateHandle();
  std::vector<std::string> texture_paths;
  render::GeometryTransferer& geometry_transferer = object_parser->Parse(io::BasePath() / "obj/tommy/tommy.obj", texture_paths);
  std::vector<std::unique_ptr<render::TextureTransferer>> texture_transferers;
  texture_transferers.reserve(texture_paths.size());
  for (const std::string& texture_path : texture_paths) {
    texture_transferers.emplace_back(std::make_unique<sdl::TextureTransferer>(texture_path));
  }
  render::Object& object = render_api->LoadObject(geometry_transferer, texture_transferers);

  *appstate = new App(window, std::move(plugin), std::move(render_api), std::move(object_parser_plugin), std::move(object_parser), object);

  sdl_guard.release();
  LOG_INFO("Application started");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event* event) {
  const auto app = static_cast<App*>(appstate);
  switch (event->type) {
    case SDL_EVENT_QUIT:
      app->app_quit = SDL_APP_SUCCESS;
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      app->render_api->OnResize(event->window.data1, event->window.data2);
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  const auto app = static_cast<App*>(appstate);
  app->render_api->RenderFrame();

  static auto start_time = std::chrono::high_resolution_clock::now();

  const auto current_time = std::chrono::high_resolution_clock::now();
  const float time = std::chrono::duration<float>(current_time - start_time).count();

  render::Uniforms uniforms = {};
  uniforms.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  uniforms.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  int width, height;
  SDL_GetWindowSize(app->window, &width, &height);
  uniforms.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 10.0f);
  uniforms.proj[1][1] *= -1;

  app->render_object.UpdateUniforms(&uniforms);

  return app->app_quit;
}

void SDL_AppQuit(void* appstate, const SDL_AppResult result) {
  const auto app = static_cast<App*>(appstate);
  delete app;

  LOG_INFO("Application is exited with status: %d", result);
  SDL_Quit();
}

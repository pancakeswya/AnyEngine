#include "app.h"

#include "resource/scope_exit.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>

namespace {

inline float GetScaleFactor() {
  return SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
}

SDL_Window* InitAndCreateWindow(
  const char* title,
  const int width,
  const int height,
  float& scale_factor,
  const SDL_WindowFlags flags
) {
  if (!SDL_Init(SDL_INIT_VIDEO)){
    throw App::Error("SDL_Init failed").WithMessage();
  }
  resource::scope_exit sdl_guard([] {
    SDL_Quit();
  });
  scale_factor = GetScaleFactor();
  SDL_Window* window = SDL_CreateWindow(
    title,
    static_cast<int>(static_cast<float>(width) * scale_factor),
    static_cast<int>(static_cast<float>(height) * scale_factor),
    flags
  );
  if (window == nullptr) {
    throw App::Error("Failed to create window").WithMessage();
  }
  sdl_guard.release();
  return window;
}

} // namespace

App::App()
  : window_(InitAndCreateWindow(
      kTitle.data(),
      kWidth,
      kHeight,
      scale_factor_,
      SDL_WINDOW_RESIZABLE |
      SDL_WINDOW_HIGH_PIXEL_DENSITY |
      sdl::Render3D::kWindowFlags
  )),
  render_(window_, scale_factor_),
  object_(render_.LoadObject("obj/Madara Uchiha/obj/Madara_Uchiha.obj")) {}

App::~App() { SDL_Quit(); }

SDL_AppResult App::HandleEvent(const SDL_Event* event) {
  render_.ProcessEvent(event);
  switch (event->type) {
    case SDL_EVENT_TERMINATING:
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult App::Iterate() {
  render_.RenderFrame();
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

#ifndef APP_H_
#define APP_H_

#include "base/error.h"
#include "render/api_plugin_import.h"
#include "render/object.h"
#include "render/object_parser_plugin_import.h"

#include <SDL3/SDL.h>

class App {
public:
  static constexpr std::string_view kTitle = "3D Render";

  static App* Cast(void *appstate) {
    return static_cast<App*>(appstate);
  }

  struct Error final : base::Error {
    using base::Error::Error;
  };

  App();
  ~App() = default;

  void Init();

  [[nodiscard]] SDL_AppResult HandleEvent(const SDL_Event* event) const;
  [[nodiscard]] SDL_AppResult Iterate();
private:
  float scale_factor_;

  render::ApiPlugin api_plugin_;
  SDL_Window* window_;

  render::ApiHandle api_handle_;

  render::ObjectParserPlugin object_parser_plugin_;
  render::ObjectParserHandle object_parser_handle_;

  render::Object* object_ = nullptr;
};

#endif // APP_H_

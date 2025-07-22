#ifndef RENDER_API_PLUGIN_IMPORT_H_
#define RENDER_API_PLUGIN_IMPORT_H_

#include "base/decl.h"
#include "base/plugin.h"
#include "render/api_plugin.h"

namespace render {

class ApiPlugin final : base::Plugin {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(ApiPlugin);

  explicit ApiPlugin(const std::string& path)
    : Plugin(path),
      create_api_and_window_(LOAD_SYM(PluginRenderCreateApiAndWindow)),
      destroy_(LOAD_SYM(PluginRenderDestroy)) {}

  [[nodiscard]] Api* CreateApiAndWindow(
    const char* title,
    const int width,
    const int height,
    const SDL_WindowFlags window_flags,
    SDL_Window*& window
  ) const {
    return create_api_and_window_(title, width, height, window_flags, window);
  }

  void Destroy(Api* api, SDL_Window* window) const {
    destroy_(api, window);
  }
private:
  DECLARE_SYM(PluginRenderCreateApiAndWindow) create_api_and_window_ = nullptr;
  DECLARE_SYM(PluginRenderDestroy) destroy_ = nullptr;
};

} // namespace render

#endif // RENDER_API_PLUGIN_IMPORT_H_

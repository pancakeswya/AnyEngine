#ifndef RENDER_API_PLUGIN_IMPORT_H_
#define RENDER_API_PLUGIN_IMPORT_H_

#include "base/decl.h"
#include "base/plugin.h"
#include "render/api_plugin.h"

namespace render {

using ApiHandle = base::PluginHandle<Api>;

class ApiPlugin final : base::Plugin {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(ApiPlugin);

  explicit ApiPlugin(const std::string& path)
    : Plugin(path),
      api_create_(LOAD_SYM(PluginRenderApiCreate)),
      api_destroy_(LOAD_SYM(PluginRenderApiDestroy)),
      api_get_window_flags_(LOAD_SYM(PluginRenderApiGetWindowFlags)) {}

  [[nodiscard]] SDL_WindowFlags GetWindowFlags() const {
    return api_get_window_flags_();
  }

  [[nodiscard]] ApiHandle CreateHandle(SDL_Window* window) const {
    return {api_create_(window), api_destroy_};
  }
private:
  DECLARE_SYM(PluginRenderApiCreate) api_create_ = nullptr;
  DECLARE_SYM(PluginRenderApiDestroy) api_destroy_ = nullptr;
  DECLARE_SYM(PluginRenderApiGetWindowFlags) api_get_window_flags_ = nullptr;
};

} // namespace render

#endif // RENDER_API_PLUGIN_IMPORT_H_

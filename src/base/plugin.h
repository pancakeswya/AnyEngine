#ifndef BASE_PLUGIN_H_
#define BASE_PLUGIN_H_

#include <SDL3/SDL_loadso.h>

#include "base/error.h"

#include <utility>
#include <memory>
#include <string>
#include <functional>

#ifdef _WIN32
#   define DLL_EXT ".dll"
#else
#   ifdef __APPLE__
#       define DLL_EXT ".dylib"
#   else
#       define DLL_EXT ".so"
#   endif
#endif

#define DECLARE_SYM(sym) decltype(&sym)
#ifdef __EMSCRIPTEN__
#define LOAD_SYM(sym) sym
#else
#define LOAD_SYM(sym) Plugin::Load<DECLARE_SYM(sym)>(#sym)
#endif

namespace base {

template <typename HandleType>
using PluginHandle = std::unique_ptr<HandleType,std::function<void(HandleType*)>>;

class Plugin {
public:
  Plugin() = default;

  explicit Plugin(const std::string& path) {
#ifndef __EMSCRIPTEN__
    if (shared_object_ = SDL_LoadObject((path + DLL_EXT).c_str()); shared_object_ == nullptr) {
      throw Error("Failed to load library: " + path + DLL_EXT", error: " + SDL_GetError());
    }
#endif
  }

  Plugin(const Plugin& other) = delete;

  Plugin(Plugin&& other) noexcept
    : shared_object_(std::exchange(other.shared_object_, nullptr)) {}

  virtual ~Plugin() {
    if (shared_object_ != nullptr) {
      SDL_UnloadObject(shared_object_);
      shared_object_ = nullptr;
    }
  }

  Plugin& operator=(Plugin&& other) noexcept {
    if (this != &other) {
      shared_object_ = std::exchange(other.shared_object_, nullptr);
    }
    return *this;
  }

  Plugin& operator=(const Plugin& other) = delete;
#ifndef __EMSCRIPTEN__
  template <typename T>
  T Load(const std::string& sym_name) const {
    auto proc = SDL_LoadFunction(shared_object_, sym_name.c_str());
    if (proc == nullptr) {
      throw Error("Failed to load symbol '" + sym_name + "'");
    }
    return reinterpret_cast<T>(proc);
  }
#endif
protected:
  SDL_SharedObject* shared_object_ = nullptr;
};

} // namespace base

#undef DLL_EXT

#endif // BASE_PLUGIN_H_

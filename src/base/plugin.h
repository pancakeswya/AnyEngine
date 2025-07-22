#ifndef BASE_PLUGIN_H_
#define BASE_PLUGIN_H_

#ifndef PLUGIN_LOAD_STATIC
#include <SDL3/SDL_loadso.h>

#include "base/error.h"

#include <utility>
#endif
#include <string>

#ifndef PLUGIN_LOAD_STATIC
#ifdef _WIN32
#   define DLL_EXT ".dll"
#else
#   ifdef __APPLE__
#       define DLL_EXT ".dylib"
#   else
#       define DLL_EXT ".so"
#   endif
#endif
#endif

#define DECLARE_SYM(sym) decltype(&sym)
#ifdef PLUGIN_LOAD_STATIC
#define LOAD_SYM(sym) sym
#else
#define LOAD_SYM(sym) Plugin::Load<DECLARE_SYM(sym)>(#sym)
#endif

namespace base {

class Plugin {
public:
  Plugin() = default;

  explicit Plugin(const std::string& path) {
#ifndef PLUGIN_LOAD_STATIC
    if (shared_object_ = SDL_LoadObject((path + DLL_EXT).c_str()); shared_object_ == nullptr) {
      throw Error("Failed to load library: " + path + DLL_EXT", error: " + SDL_GetError());
    }
#endif
  }

  Plugin(const Plugin& other) = delete;

  Plugin(Plugin&& other) noexcept
#ifdef PLUGIN_LOAD_STATIC
  = default;
#else
    : shared_object_(std::exchange(other.shared_object_, nullptr)) {}
#endif

  virtual ~Plugin()
#ifdef PLUGIN_LOAD_STATIC
  = default;
#else
  {
    if (shared_object_ != nullptr) {
      SDL_UnloadObject(shared_object_);
      shared_object_ = nullptr;
    }
  }
#endif

  Plugin& operator=(Plugin&& other) noexcept
#ifdef PLUGIN_LOAD_STATIC
  = default;
#else
  {
    if (this != &other) {
      shared_object_ = std::exchange(other.shared_object_, nullptr);
    }
    return *this;
  }
#endif

  Plugin& operator=(const Plugin& other) = delete;
#ifndef PLUGIN_LOAD_STATIC
  template <typename T>
  T Load(const std::string& sym_name) const {
    auto proc = SDL_LoadFunction(shared_object_, sym_name.c_str());
    if (proc == nullptr) {
      throw Error("Failed to load symbol '" + sym_name + "', error: " + SDL_GetError());
    }
    return reinterpret_cast<T>(proc);
  }
protected:
  SDL_SharedObject* shared_object_ = nullptr;
#endif
};

} // namespace base

#ifdef DLL_EXT
#undef DLL_EXT
#endif

#endif // BASE_PLUGIN_H_

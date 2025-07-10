#ifndef BASE_PLUGIN_H_
#define BASE_PLUGIN_H_

#include <SDL3/SDL_loadso.h>

#include "base/error.h"

#include <algorithm>
#include <memory>
#include <string>

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
#define LOAD_SYM(sym) Plugin::Load<DECLARE_SYM(sym)>(#sym)

namespace base {

template <typename HandleType>
using PluginHandle = std::unique_ptr<HandleType, void(*)(HandleType*)>;

class Plugin {
public:
  Plugin() = default;

  explicit Plugin(const std::string& path) {
    if (shared_object_ = SDL_LoadObject((path + DLL_EXT).c_str()); shared_object_ == nullptr) {
      throw Error("Failed to load library: " + path);
    }
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

  template <typename T>
  T Load(const std::string& sym_name) const {
    auto proc = SDL_LoadFunction(shared_object_, sym_name.c_str());
    if (proc == nullptr) {
      throw Error("Failed to load symbol '" + sym_name + "'");
    }
    return reinterpret_cast<T>(proc);
  }
protected:
  SDL_SharedObject* shared_object_ = nullptr;
};

} // namespace base

#undef DLL_EXT

#endif // BASE_PLUGIN_H_

#include "fs/path.h"

#include "fs/error.h"

#include <SDL3/SDL_filesystem.h>

namespace fs {

std::filesystem::path BasePath() {
#if __ANDROID__
  std::filesystem::path base_path = "";
#else
  const char* base_path_ptr = SDL_GetBasePath();
  if (base_path_ptr == nullptr) {
    throw Error("SDL_GetBasePath failed");
  }
  const std::filesystem::path base_path = base_path_ptr;
#endif
  return base_path;
}

} // namespace fs
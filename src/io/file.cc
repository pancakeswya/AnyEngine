#include "io/file.h"

#include "io/error.h"
#include "resource/scope_exit.h"

#include <SDL3/SDL.h>

namespace io {

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

std::vector<char> ReadFile(const std::string& path) {
  SDL_IOStream* file = SDL_IOFromFile(path.data(), "rb");
  if (!file) {
    throw Error("Could not open file: " + path);
  }
  resource::scope_exit file_guard([file] {
    SDL_CloseIO(file);
  });
  const Sint64 file_size = SDL_GetIOSize(file);
  if (file_size < 0) {
    throw Error("Could not determine file size");
  }
  std::vector<char> buffer(file_size);
  if (const size_t bytes_read = SDL_ReadIO(file, buffer.data(), file_size); bytes_read != static_cast<size_t>(file_size)) {
    throw Error("Could not read file");
  }
  return buffer;
}

} // namespace io
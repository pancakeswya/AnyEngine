#include "fs/file.h"

#include "fs/error.h"
#include "resource/scope_exit.h"

#include <SDL3/SDL.h>

namespace fs {

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

} // namespace fs
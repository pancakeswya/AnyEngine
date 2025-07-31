#ifndef SDL_IMAGE_CONVERT_H_
#define SDL_IMAGE_CONVERT_H_

#include "render/texture_mapper.h"

#include <string>
#include <memory>
#include <vector>
#include <filesystem>
#include <type_traits>

namespace sdl {

template<typename TextureMapperInterface, typename TextureMapperImpl, typename TextureMapperMockImpl>
std::vector<std::unique_ptr<TextureMapperInterface>> ConvertPathsToMappers(const std::vector<std::string>& texture_paths) {
  static_assert(
    std::is_base_of_v<render::TextureMapper, TextureMapperInterface> &&
    std::is_base_of_v<TextureMapperInterface, TextureMapperImpl> &&
    std::is_base_of_v<TextureMapperInterface, TextureMapperMockImpl>
  );
  std::vector<std::unique_ptr<TextureMapperInterface>> texture_mappers;
  texture_mappers.reserve(texture_paths.size());
  if (texture_paths.empty()) {
    texture_mappers.emplace_back(std::make_unique<TextureMapperMockImpl>());
  } else {
    for (const std::string& texture_path : texture_paths) {
      try {
        std::filesystem::path texture_filepath(texture_path);
        texture_mappers.emplace_back(std::make_unique<TextureMapperImpl>(texture_filepath.lexically_normal().string()));
      } catch (...) {
        texture_mappers.emplace_back(std::make_unique<TextureMapperMockImpl>());
      }
    }
  }
  return texture_mappers;
}

} // namespace sdl

#endif // SDL_IMAGE_CONVERT_H_

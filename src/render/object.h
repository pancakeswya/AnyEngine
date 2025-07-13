#ifndef RENDER_OBJECT_H_
#define RENDER_OBJECT_H_

#include "render/geometry.h"
#include "render/texture.h"

#include <utility>
#include <vector>

namespace render {

class Object {
public:
  Object() noexcept = default;

  Object(
    const GeometryInfo& info,
    const std::vector<TextureIndices>& texture_indices
  ) : info_(info), texture_indices_(texture_indices) {}

  Object(const Object& other) = delete;

  Object(Object&& other) noexcept
    : info_(std::exchange(other.info_, {})),
      texture_indices_(std::move(other.texture_indices_)) {}

  virtual ~Object() = default;

  Object& operator=(const Object& other) = delete;

  Object& operator=(Object&& other) noexcept {
    if (this != &other) {
      info_ = std::exchange(other.info_, {});
      texture_indices_ = std::move(other.texture_indices_);
    }
    return *this;
  }

  [[nodiscard]] const GeometryInfo& info() const noexcept {
    return info_;
  }

  [[nodiscard]] const std::vector<TextureIndices>& texture_indices() const noexcept {
    return texture_indices_;
  }

  virtual void UpdateUniforms(const Uniforms* uniforms) = 0;
private:
  GeometryInfo info_ = {};
  std::vector<TextureIndices> texture_indices_;
};

} // namespace render

#endif // RENDER_OBJECT_H_

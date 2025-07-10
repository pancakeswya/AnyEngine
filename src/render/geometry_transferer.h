#ifndef RENDER_GEOMETRY_TRANSFERER_H_
#define RENDER_GEOMETRY_TRANSFERER_H_

#include "render/geometry.h"
#include "render/texture.h"

#include <vector>

namespace render {

struct GeometryTransferer {
  virtual ~GeometryTransferer() = default;
  [[nodiscard]] virtual GeometryInfo info() const noexcept = 0;
  [[nodiscard]] virtual const std::vector<TextureIndices>& texture_indices() const noexcept = 0;
  virtual void Transfer(Vertex* vertices, Index* indices) = 0;
};

} //  namespace render

#endif // RENDER_GEOMETRY_TRANSFERER_H_

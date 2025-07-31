#ifndef RENDER_API_H_
#define RENDER_API_H_

#include "render/geometry_transferer.h"
#include "render/texture_mapper.h"
#include "render/object.h"

#include <vector>
#include <memory>
#include <type_traits>

namespace render {

template<typename TextureMapperImpl>
struct Api {
  static_assert(std::is_base_of_v<TextureMapper, TextureMapperImpl>);

  virtual ~Api() = default;
  virtual void RenderFrame() = 0;
  virtual void OnResize(int width, int height) = 0;
  virtual Object* LoadObject(
    GeometryTransferer& geometry_transferer,
    std::vector<std::unique_ptr<TextureMapperImpl>>& texture_mappers
  ) = 0;
};

} // namespacer render

#endif // RENDER_API_H_

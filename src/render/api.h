#ifndef RENDER_API_H_
#define RENDER_API_H_

#include "render/object.h"
#include "render/geometry_transferer.h"
#include "render/texture_transferer.h"

namespace render {

struct Api {
  virtual ~Api() = default;

  virtual void RenderFrame() = 0;
  virtual Object& LoadObject(
      GeometryTransferer& geometry_transferer,
      std::vector<std::unique_ptr<TextureTransferer>>& texture_transferers
  ) = 0;
  virtual void OnResize(int width, int height) = 0;
};

} // namespace render

#endif // RENDER_API_H_

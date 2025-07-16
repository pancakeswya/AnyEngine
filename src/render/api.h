#ifndef RENDER_API_H_
#define RENDER_API_H_

#include "render/object.h"
#include "render/geometry_transferer.h"
#include "render/texture_transferer.h"
#include "render/gui.h"

#include <memory>
#include <vector>

namespace render {

struct Api {
  virtual ~Api() = default;

  virtual void RenderFrame() = 0;
  virtual Object* LoadObject(
      GeometryTransferer& geometry_transferer,
      std::vector<std::unique_ptr<TextureMapper>>& texture_mappers
  ) = 0;
  virtual void OnResize(int width, int height) = 0;

  virtual GuiRenderer* GetGuiRenderer() noexcept = 0;
};

} // namespace render

#endif // RENDER_API_H_

#ifndef RENDER_PARSERS_OBJ_TRANSFERER_H_
#define RENDER_PARSERS_OBJ_TRANSFERER_H_

#include "render/parsers/obj/types.h"
#include "render/geometry_transferer.h"

namespace obj {

class GeometryTransferer final : public render::GeometryTransferer {
public:
  explicit GeometryTransferer(Data&& data) noexcept;

  GeometryTransferer() noexcept = default;
  GeometryTransferer(const Data& data) noexcept = delete;
  GeometryTransferer(GeometryTransferer&& other) noexcept = default;
  ~GeometryTransferer() noexcept override = default;

  GeometryTransferer& operator=(const GeometryTransferer& other) noexcept = delete;
  GeometryTransferer& operator=(GeometryTransferer&& other) noexcept = default;

  [[nodiscard]] render::GeometryInfo info() const noexcept override;
  [[nodiscard]] const std::vector<render::TextureIndices>& texture_indices() const noexcept override;
  void Transfer(render::Vertex* vertices, render::Index* indices) override;
private:
  Data data_;
};

} // namespace obj

#endif // RENDER_PARSERS_OBJ_TRANSFERER_H_

#include "render/parsers/obj/transferer.h"

#include <unordered_map>

namespace obj {

GeometryTransferer::GeometryTransferer(Data&& data) noexcept : data_(std::move(data)) {}

render::GeometryInfo GeometryTransferer::info() const noexcept {
  return {
    .vertex_count = data_.indices.size(),
    .index_count = data_.indices.size()
  };
}

const std::vector<render::TextureIndices>& GeometryTransferer::texture_indices() const noexcept {
  return data_.usemtl;
}

void GeometryTransferer::Transfer(render::Vertex* vertices, render::Index* indices) {
  std::unordered_map<Indices, unsigned int, Indices::Hash> index_map;

  unsigned int next_combined_idx = 0, combined_idx = 0;
  for (const Indices& index : data_.indices) {
    if (index_map.contains(index)) {
      combined_idx = index_map[index];
    } else {
      combined_idx = next_combined_idx;
      index_map.emplace(index, combined_idx);
      const unsigned int i_v = index.fv * 3, i_n = index.fn * 3, i_t = index.ft * 2;
      glm::vec3 normal = {};
      glm::vec2 texture = {};
      if (!data_.vn.empty()) {
        normal.x = data_.vn[i_n];
        normal.y = data_.vn[i_n + 1];
        normal.z = data_.vn[i_n + 2];
      }
      if (!data_.vt.empty()) {
        texture.x = data_.vt[i_t];
        texture.y = data_.vt[i_t + 1];
      }
      *vertices++ = render::Vertex{
        glm::vec3(data_.v[i_v], data_.v[i_v + 1], data_.v[i_v + 2]),
        normal,
        texture,
      };
      ++next_combined_idx;
    }
    *indices++ = combined_idx;
  }
}

} // namespace obj
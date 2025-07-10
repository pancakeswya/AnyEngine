#ifndef RENDER_GEOMETRY_H_
#define RENDER_GEOMETRY_H_

#include <glm/glm.hpp>

#include <cstdint>

namespace render {

struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 tex_coord;
};

using Index = uint32_t;

struct Uniforms {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

struct GeometryInfo {
  size_t vertex_count;
  size_t index_count;
};

struct Geometry {
  Vertex* vertices;
  Index* indices;
  GeometryInfo info;
};

} // namespace render

#endif // RENDER_GEOMETRY_H_

#ifndef RENDER_OBJECT_PARSER_H_
#define RENDER_OBJECT_PARSER_H_

#include "render/geometry_transferer.h"

#include <string>
#include <vector>

namespace render {

struct ObjectParser {
  virtual ~ObjectParser() = default;
  virtual GeometryTransferer& Parse(const std::string& path, std::vector<std::string>& texture_paths) = 0;
};

} // namespace render

#endif // RENDER_OBJECT_PARSER_H_

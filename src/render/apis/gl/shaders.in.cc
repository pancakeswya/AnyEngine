#include "render/apis/gl/shaders.h"

namespace gl {

std::vector<ShaderInfo> GetShaderInfos() {
  return {
     {
       R"(@simple.vert@)",
       GL_VERTEX_SHADER
     },
     {
       R"(@simple.frag@)",
       GL_FRAGMENT_SHADER
     }
  };
}

} // namespace gl
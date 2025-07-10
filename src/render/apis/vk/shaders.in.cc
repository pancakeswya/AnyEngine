#include "render/apis/vk/shaders.h"

#include "render/apis/vk/error.h"

#include <shaderc/shaderc.hpp>

namespace vk {

namespace {

inline std::vector<uint32_t> CompileToSpv(const shaderc::Compiler& compiler, shaderc_shader_kind kind, const char* source) {
  const shaderc::SpvCompilationResult spirv = compiler.CompileGlslToSpv(source, strlen(source), kind, "source");
  if (spirv.GetCompilationStatus() != shaderc_compilation_status_success) {
    throw Error("failed to compile shaders: " + spirv.GetErrorMessage());
  }
  return {spirv.cbegin(), spirv.cend()};
}

} // namespace

std::vector<Shader> CompileShaders(VkDevice device) {
  shaderc::Compiler compiler;
  std::vector<Shader> shaders;
  shaders.reserve(2);
  shaders.emplace_back(
    device,
    CompileToSpv(compiler, shaderc_vertex_shader, R"(@simple.vert@)"),
    VK_SHADER_STAGE_VERTEX_BIT,
    "main"
  );
  shaders.emplace_back(
    device,
    CompileToSpv(compiler, shaderc_fragment_shader, R"(@simple.frag@)"),
    VK_SHADER_STAGE_FRAGMENT_BIT,
    "main"
  );
  return shaders;
}

} // namespace vk
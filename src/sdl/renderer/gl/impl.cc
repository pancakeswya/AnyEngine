#include "sdl/renderer/gl/impl.h"

#include "sdl/image/convert.h"
#include "sdl/renderer/gl/error.h"
#include "sdl/renderer/gl/texture_mapper.h"
#include "render/apis/gl/loader.h"
#include "render/apis/gl/check.h"

#include <SDL3/SDL_log.h>

namespace sdl::gl {

namespace {

SDL_GLContext CreateContext(SDL_Window* window) {
#ifdef USE_OPENGL_ES3
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
# if defined(__APPLE__)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
# else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
# endif
#endif
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (context == nullptr) {
    throw Error(std::string("Failed to create SDL context with error:") + SDL_GetError());
  }
  if (!::gl::InitLoader(SDL_GL_GetProcAddress)) {
    throw Error("Failed to load GL/GLES");
  }
#ifndef ANY_RELEASE
  gl_log_callback = SDL_Log;
#endif
  SDL_Log("Successfully loaded GL/GLES, version: %s\n", glGetString(GL_VERSION));
  return context;
}

} // namespace

RendererImpl::RendererImpl(SDL_Window* window, const float scale_factor)
  : window_(window),
    context_(CreateContext(window)),
    renderer_(window_),
    gui_renderer_(
      window,
      context_,
      ::gl::Api::kGlSlVersion,
      scale_factor
    ) {
  SDL_GL_MakeCurrent(window, context_);
  SDL_GL_SetSwapInterval(1);
}

RendererImpl::~RendererImpl() {
  SDL_GL_DestroyContext(context_);
}

void RendererImpl::RenderFrame() {
  gui_renderer_.RenderFrame();
  renderer_.RenderFrame();
  GuiRenderer::Draw();
  SDL_GL_SwapWindow(window_);
}

render::Object* RendererImpl::LoadObject(
    render::GeometryTransferer& geometry_transferer,
    const std::vector<std::string>& texture_paths) {
  std::vector texture_mappers = ConvertPathsToMappers<::gl::TextureMapper, TextureMapper, TextureMapperMock>(texture_paths);
  return renderer_.LoadObject(geometry_transferer, texture_mappers);
}

void RendererImpl::OnResize(const int width, const int height) {
  renderer_.OnResize(width, height);
}

} // namespace sdl::gl
#include "app.h"

#include "resource/scope_exit.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdexcept>

#define TRY_WITH_EXIT_ON_CATCH(EXPR)                                  \
  try { EXPR; }                                                       \
  catch (const std::exception& e) {                                    \
    SDL_Log("Caught exception, what(): %s", e.what());                \
    return SDL_APP_FAILURE;                                           \
  }

SDL_AppResult SDL_AppInit(
  void** appstate,
  [[maybe_unused]]int argc,
  [[maybe_unused]]char* argv[]
) {
  if (!SDL_Init(SDL_INIT_VIDEO)){
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  resource::scope_exit sdl_guard([] {
    SDL_Quit();
  });
  TRY_WITH_EXIT_ON_CATCH(
    const auto app = new App;
    TRY_WITH_EXIT_ON_CATCH (
      app->Init();
      *appstate = app;
    )
  )
  sdl_guard.release();
  SDL_Log("Application started");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event* event) TRY_WITH_EXIT_ON_CATCH(
  return App::Cast(appstate)->HandleEvent(event);
)

SDL_AppResult SDL_AppIterate(void *appstate) TRY_WITH_EXIT_ON_CATCH(
  return App::Cast(appstate)->Iterate();
)

void SDL_AppQuit(void* appstate, const SDL_AppResult result) {
  delete App::Cast(appstate);

  SDL_Log("Application is exited with status: %d", result);
  SDL_Quit();
}

#include "app.h"

#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>

#include <stdexcept>

#define TRY_WITH_EXIT_ON_CATCH(EXPR)                                  \
  try { EXPR; }                                                       \
  catch (const std::exception& e) {                                   \
    SDL_Log("Caught exception, what(): %s", e.what());                \
    return SDL_APP_FAILURE;                                           \
  }

SDL_AppResult SDL_AppInit(
  void** appstate,
  [[maybe_unused]]int argc,
  [[maybe_unused]]char* argv[]
) {
  TRY_WITH_EXIT_ON_CATCH(
    *appstate = new App;
  )
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
}

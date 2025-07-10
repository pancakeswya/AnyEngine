#include "app.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdexcept>

#define CATCH_LOGGED(expr) catch(const std::exception& e) { SDL_Log("Caught exception, what(): %s", e.what()); expr; }

SDL_AppResult SDL_AppInit(
  void** appstate,
  [[maybe_unused]]int argc,
  [[maybe_unused]]char* argv[]
) {
  if (!SDL_Init(SDL_INIT_VIDEO)){
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  try {
    *appstate = new App;
  } CATCH_LOGGED (
    SDL_Quit();
    return SDL_APP_FAILURE;
  )
  SDL_Log("Application started");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event* event) try {
  return App::Cast(appstate)->HandleEvent(event);
} CATCH_LOGGED (
  return SDL_APP_FAILURE;
)

SDL_AppResult SDL_AppIterate(void *appstate) try {
  return App::Cast(appstate)->Iterate();
} CATCH_LOGGED (
  return SDL_APP_FAILURE;
)

void SDL_AppQuit(void* appstate, const SDL_AppResult result) {
  delete App::Cast(appstate);

  SDL_Log("Application is exited with status: %d", result);
  SDL_Quit();
}

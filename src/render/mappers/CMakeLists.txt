option(BUILD_SDL_TEXTURE_TRANSFERER "Build sdl texture transferer" ON)

if (BUILD_SDL_TEXTURE_TRANSFERER)
    add_subdirectory(sdl)
endif()

add_subdirectory(mock)
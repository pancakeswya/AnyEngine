if (NOT TARGET SDL3::SDL3)
    find_package(SDL3 REQUIRED)
endif()

if (NOT TARGET SDL3_image::SDL3_image)
    include(FetchContent)

    set(SDLIMAGE_VENDORED ON)
    set(SDLIMAGE_AVIF OFF)
    set(SDLIMAGE_BMP OFF)
    set(SDLIMAGE_JPEG OFF)
    set(SDLIMAGE_WEBP OFF)

    FetchContent_Declare(
            SDL_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG 18486b134e7e753935b0e66556d2e9f7634e44a7
    )
    FetchContent_MakeAvailable(SDL_image)
endif()

add_library(sdl_image STATIC)
target_sources(sdl_image PUBLIC
        error.h
        convert.h
        texture_mapper.h
        texture_mapper.cc
        texture_mapper_mock.h
)

if (APPLE AND NOT BUILD_SHARED_LIBS)
    find_library(IO_LIB ImageIO REQUIRED)
    find_library(CS_LIB CoreServices REQUIRED)
    find_library(CT_LIB CoreText REQUIRED)
    find_library(CG_LIB CoreGraphics REQUIRED)
    find_library(CF_LIB CoreFoundation REQUIRED)
    target_link_libraries(sdl_image PUBLIC ${CF_LIB} ${CT_LIB} ${IO_LIB} ${CS_LIB} ${CG_LIB})
endif()

target_link_libraries(sdl_image PUBLIC SDL3::SDL3 SDL3_image::SDL3_image render)
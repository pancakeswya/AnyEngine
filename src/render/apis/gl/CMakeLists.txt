include(${CMAKE_SOURCE_DIR}/cmake/plugins.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/shaders.cmake)

configure_shaders(shaders shaders.in.cc shaders.cc)

make_plugin(gl_api
        api.h
        api.cc
        api_plugin.cc
        context.h
        context.cc
        handle.h
        loader.h
        error.h
        error.cc
        object.h
        object.cc
        buffer.h
        texture.h
        texture.cc
        shader.h
        shader.cc
        shaders.h
        shaders.cc
)

if (NOT TARGET OpenGL::GL)
    find_package(OpenGL REQUIRED)
endif()

if (NOT TARGET SDL3::SDL3)
    find_package(SDL3 REQUIRED)
endif()

target_link_libraries(gl_api PUBLIC SDL3::SDL3 OpenGL::GL render)
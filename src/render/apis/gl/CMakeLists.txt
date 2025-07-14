include(FetchContent)
include(${CMAKE_SOURCE_DIR}/cmake/plugins.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/shaders.cmake)

configure_shaders(shaders shaders.in.cc shaders.cc)

make_plugin(gl_api
        api.h
        api.cc
        api_plugin.cc
        context.h
        context.cc
        check.h
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

if (ANDROID OR EMSCRIPTEN)
    set(GLAD_GL_ARG gles2:core=3.0)
    target_compile_definitions(gl_api PUBLIC RENDER_OPENGL_ES3)
    if (ANDROID)
        target_compile_definitions(gl_api PUBLIC USE_BUFFER_MAP)
        target_link_libraries(gl_api PUBLIC GLESv3)
    endif()
else()
    find_package(OpenGL REQUIRED)
    set(GLAD_GL_ARG gl:core=3.3)
    target_compile_definitions(gl_api PUBLIC USE_BUFFER_MAP)
    target_link_libraries(gl_api PUBLIC OpenGL::GL)
endif()

if (NOT TARGET SDL3::SDL3)
    find_package(SDL3 REQUIRED)
endif()

if (NOT TARGET glad)
    FetchContent_Declare(
            glad
            GIT_REPOSITORY https://github.com/Dav1dde/glad.git
            GIT_TAG v2.0.8
            SOURCE_SUBDIR cmake
    )
    FetchContent_MakeAvailable(glad)
    glad_add_library(glad STATIC API ${GLAD_GL_ARG} LOCATION ${CMAKE_BINARY_DIR}/glad-build/${TARGET})
endif()

target_compile_definitions(gl_api PUBLIC USE_VAO)
target_link_libraries(gl_api PUBLIC SDL3::SDL3 glad render)